#include "board.h"
#include "combinator.h"
#include "pawn-cache.h"
#include "seven-tuple.h"
#include "six-tuple.h"
#include "std.h"

// The way the spaces on the checkerboard are ordered on a printed page.
const int human_readable_ordering[] = {
  28, 29, 30, 31,
  24, 25, 26, 27,
  20, 21, 22, 23,
  16, 17, 18, 19,
  12, 13, 14, 15,
  8, 9, 10, 11,
  4, 5, 6, 7,
  0, 1, 2, 3
};

const char human_readable_chars[] = "-WBwb";

// Stores a precomputed table of offsets for checkers moves and jumps. These
// are used for fast move generation.
int move_offsets[32][4];
int jump_offsets[32][4];

// Precompute a table of offsets for checkers moves and jumps. This will be
// used later for fast move generation.
bool DoBoardPrecomputations() {
  int dx = -1;
  int dy = 1;
  for (int i = 0; i < 32; ++i) {
    for (int j = 0; j < 4; ++j) {
      int x;
      int y;
      Board::SpaceNumberToXY(i, &x, &y);
      move_offsets[i][j] = Board::IsOnBoard(x + dx, y + dy) ?
        Board::XYToSpaceNumber(x + dx, y + dy) : -1;
      jump_offsets[i][j] = Board::IsOnBoard(x + 2 * dx, y + 2 * dy) ?
        Board::XYToSpaceNumber(x + 2 * dx, y + 2 * dy) : -1;
      // Rotate the direction 90 degrees clockwise.
      int tmp = dy;
      dy = -dx;
      dx = tmp;
    }
  }
  return true;
}

// This line make the precomputations run before main().
bool done_board_precomputations = DoBoardPrecomputations();

Board::Board() {
  Clear();
}

Board::Board(const Board& b) {
  for (int i = 0; i < 32; ++i) {
    pieces[i] = b.pieces[i];
  }
}

Board::Board(const string& s) {
  int pieces_found = 0;
  for (int i = 0; i < s.length(); ++i) {
    char c = s[i];
    for (int j = 0; j < 5; ++j) {
      if (c == human_readable_chars[j]) {
        const int index = human_readable_ordering[pieces_found];
        pieces[index] = (Piece)j;
        ++pieces_found;
        break;
      }
    }
  }
  if (pieces_found != 32) {
    throw "Error parsing a checkerboard: incorrect number of squares.";
  }
}

Board::Board(const SevenTuple& index) {
  Deindex(index.GetDB(), index.GetIndex());
}

Board::Board(const SixTuple& db, uint64 index) {
  Deindex(db, index);
}

Board::Board(int nbk, int nwk, int nbp, int nwp, int rbp, int rwp,
             uint64 index) {
  SixTuple db(nbk, nwk, nbp, nwp, rbp, rwp);
  Deindex(db, index);
}

Piece Board::GetPiece(int index) const {
  return pieces[index];
}

void Board::SetPiece(int index, Piece p) {
  pieces[index] = p;
}

void Board::MovePiece(int from, int to) {
  if (from == to) {
    return;
  }
  pieces[to] = pieces[from];
  pieces[from] = Empty;
}

void Board::Clear() {
  for (int i = 0; i < 32; ++i) {
    pieces[i] = Empty;
  }
}

void Board::Clear(Piece p) {
  for (int i = 0; i < 32; ++i) {
    if (pieces[i] == p) {
      pieces[i] = Empty;
    }
  }
}

SixTuple Board::WhichDatabaseSlice() const {
  SixTuple db(0, 0, 0, 0, 0, 0);
  for (int i = 0; i < 32; ++i) {
    switch (pieces[i]) {
      case BlackPawn:
        db.rbp = i / 4;
        ++db.nbp;
        break;
      case WhitePawn:
        if (db.rwp == 0) {
          db.rwp = (31 - i) / 4;
        }
        ++db.nwp;
        break;
      case BlackKing:
        ++db.nbk;
        break;
      case WhiteKing:
        ++db.nwk;
        break;
      case Empty:
      default:
        // Do nothing.
        break;
    };
  }
  return db;
}

uint64 Board::Index(const SixTuple& db) const {
  // Calculate black pawn sub-index.
  uint64 bp_index = 0;
  int k = db.nbp - 1;
  for (int n = 4 * db.rbp + 3; n >= 0 && k >= 0; --n) {
    if (pieces[n] == BlackPawn) {
      --k;
    } else {
      bp_index += Choose(n, k);
    }
  }
  // Calculate white pawn sub-index.
  int n = 4 * db.rwp + 3;
  for (int i = 4 * (7 - db.rwp); i < 32; ++i) {
    if (pieces[i] == BlackPawn) {
      --n;
    }
  }
  uint64 wp_index = 0;
  k = db.nwp - 1;
  for (int i = 4 * (7 - db.rwp); i < 32 && k >= 0; ++i) {
    switch (pieces[i]) {
      case BlackPawn:
        break;
      case WhitePawn:
        --k;
        --n;
        break;
      default:
        wp_index += Choose(n, k);
        --n;
        break;
    };
  }
  // Calculate black king sub-index.
  uint64 bk_index = 0;
  n = 32 - db.nbp - db.nwp - 1;
  k = db.nbk - 1;
  for (int i = 0; i < 32 && k >= 0; ++i) {
    switch (pieces[i]) {
      case BlackPawn:
      case WhitePawn:
        break;
      case BlackKing:
        --k;
        --n;
        break;
      case WhiteKing:
      case Empty:
        bk_index += Choose(n, k);
        --n;
        break;
    };
  }
  // Calculate white king sub-index.
  uint64 wk_index = 0;
  n = 32 - db.nbp - db.nwp - db.nbk - 1;
  k = db.nwk - 1;
  for (int i = 0; i < 32 && k >= 0; ++i) {
    switch (pieces[i]) {
      case WhiteKing:
        --k;
        --n;
        break;
      case Empty:
        wk_index += Choose(n, k);
        --n;
        break;
      default:
        break;
    };
  }
  // Combine the four separate sub-indices into one overall index.
  const PawnCache& pc = PawnCache::Get(db);
  uint64 index = pc.SumWP(bp_index) + wp_index;
  const int bk_avail = 32 - db.nbp - db.nwp;
  index *= Choose(bk_avail, db.nbk);
  index += bk_index;
  const int wk_avail = bk_avail - db.nbk;
  index *= Choose(wk_avail, db.nwk);
  index += wk_index;
  return index;
}

SevenTuple Board::Index() const {
  SixTuple db = WhichDatabaseSlice();
  uint64 index = Index(db);
  return SevenTuple(db, index);
}

void Board::Deindex(const SixTuple& db, uint64 index) {
  Clear();
  // Use division & modulus to find the indices for the kings.
  const int bk_avail = 32 - db.nbp - db.nwp;
  const int wk_avail = bk_avail - db.nbk;
  const uint64 bk_combinations = Choose(bk_avail, db.nbk);
  const uint64 wk_combinations = Choose(wk_avail, db.nwk);
  const uint64 wkr = index / wk_combinations;
  const uint64 bkr = wkr / bk_combinations;
  const uint64 wk_index = index - wk_combinations * wkr;
  const uint64 bk_index = wkr - bk_combinations * bkr;
  // Use the pawn cache to determine the pawn indices, to avoid redundant work.
  const PawnCache& pc = PawnCache::Get(db);
  if (bkr >= pc.MaxWP()) {
    throw "The index is out of range and cannot be deindexed.";
  }
  uint64 bp_index;
  uint64 wp_index;
  pc.DecomposeIndex(bkr, &bp_index, &wp_index);
  // Place black pawns.
  const int bp_avail = 4 * (db.rbp + 1);
  vector<int> sq;
  for (int i = bp_avail - 1; i >= 0; --i) {
    sq.push_back(i);
  }
  Combinator bp(bp_avail, db.nbp);
  bp.Deindex(bp_index, BlackPawn, &sq, this);
  // Place white pawns.
  sq.clear();
  for (int i = 4 * (7 - db.rwp); i < 32; ++i) {
    if (pieces[i] == Empty) {
      sq.push_back(i);
    }
  }
  const int wp_avail = sq.size();
  Combinator wp(wp_avail, db.nwp);
  wp.Deindex(wp_index, WhitePawn, &sq, this);
  // Place black kings.
  sq.clear();
  for (int i = 0; i < 32; ++i) {
    if (pieces[i] == Empty) {
      sq.push_back(i);
    }
  }
  Combinator bk(bk_avail, db.nbk);
  bk.Deindex(bk_index, BlackKing, &sq, this);
  // Place white kings.
  sq.clear();
  for (int i = 0; i < 32; ++i) {
    if (pieces[i] == Empty) {
      sq.push_back(i);
    }
  }
  Combinator wk(wk_avail, db.nwk);
  wk.Deindex(wk_index, WhiteKing, &sq, this);
}

Board Board::Mirror() const {
  Board b;
  for (int i = 0; i < 32; ++i) {
    b.pieces[i] = MirrorPiece(pieces[31 - i]);
  }
  return b;
}

Piece Board::MirrorPiece(Piece p) {
  switch (p) {
    case BlackPawn:
      return WhitePawn;
    case WhitePawn:
      return BlackPawn;
    case BlackKing:
      return WhiteKing;
    case WhiteKing:
      return BlackKing;
    case Empty:
      return Empty;
  };
}

uint64 Board::MirrorIndex(const SixTuple& db) const {
  // Note that db is ID of the mirror database. So there is a bit of color
  // switcheroo happening here to avoid making a copy of the board.
  // Calculate black pawn sub-index.
  uint64 bp_index = 0;
  int k = db.nbp - 1;
  for (int n = 4 * db.rbp + 3; n >= 0 && k >= 0; --n) {
    if (pieces[31 - n] == WhitePawn) {
      --k;
    } else {
      bp_index += Choose(n, k);
    }
  }
  // Calculate white pawn sub-index.
  int n = 4 * db.rwp + 3;
  for (int i = 4 * (7 - db.rwp); i < 32; ++i) {
    if (pieces[31 - i] == WhitePawn) {
      --n;
    }
  }
  uint64 wp_index = 0;
  k = db.nwp - 1;
  for (int i = 4 * (7 - db.rwp); i < 32 && k >= 0; ++i) {
    switch (pieces[31 - i]) {
      case WhitePawn:
        break;
      case BlackPawn:
        --k;
        --n;
        break;
      default:
        wp_index += Choose(n, k);
        --n;
        break;
    };
  }
  // Calculate black king sub-index.
  uint64 bk_index = 0;
  n = 32 - db.nbp - db.nwp - 1;
  k = db.nbk - 1;
  for (int i = 0; i < 32 && k >= 0; ++i) {
    switch (pieces[31 - i]) {
      case BlackPawn:
      case WhitePawn:
        break;
      case WhiteKing:
        --k;
        --n;
        break;
      case BlackKing:
      case Empty:
        bk_index += Choose(n, k);
        --n;
        break;
    };
  }
  // Calculate white king sub-index.
  uint64 wk_index = 0;
  n = 32 - db.nbp - db.nwp - db.nbk - 1;
  k = db.nwk - 1;
  for (int i = 0; i < 32 && k >= 0; ++i) {
    switch (pieces[31 - i]) {
      case BlackKing:
        --k;
        --n;
        break;
      case Empty:
        wk_index += Choose(n, k);
        --n;
        break;
      default:
        break;
    };
  }
  // Combine the four separate sub-indices into one overall index.
  const PawnCache& pc = PawnCache::Get(db);
  uint64 index = pc.SumWP(bp_index) + wp_index;
  const int bk_avail = 32 - db.nbp - db.nwp;
  index *= Choose(bk_avail, db.nbk);
  index += bk_index;
  const int wk_avail = bk_avail - db.nbk;
  index *= Choose(wk_avail, db.nwk);
  index += wk_index;
  return index;
}

SevenTuple Board::MirrorIndex() const {
  SixTuple db = WhichDatabaseSlice().Mirror();
  uint64 index = MirrorIndex(db);
  return SevenTuple(db, index);
}

bool Board::IsWhite(Piece p) {
  switch (p) {
    case WhitePawn:
    case WhiteKing:
      return true;
    default:
      return false;
  };
}

bool Board::PawnCaptures(int from, set<SevenTuple>* captures) {
  return GenerateCaptures(from, 2, captures);
}

bool Board::KingCaptures(int from, set<SevenTuple>* captures) {
  return GenerateCaptures(from, 4, captures);
}

bool Board::ConversionMoves(
  int from, vector<uint64>* moves, SixTuple* db) {
  for (int direction = 0; direction < 2; ++direction) {
    const int to = move_offsets[from][direction];
    if (to < 0 || pieces[to] != Empty) {
      continue;
    }
    if (!moves) {
      return true;
    }
    pieces[from] = Empty;
    pieces[to] = to < 28 ? BlackPawn : BlackKing;
    SevenTuple mi = MirrorIndex();
    moves->push_back(mi.GetIndex());
    *db = mi.GetDB();
    pieces[to] = Empty;
    pieces[from] = BlackPawn;
  }
  if (!moves) {
    return false;
  }
  return moves->size() > 0;
}

bool Board::NonConversionMoves(
  int from, int max_direction, vector<uint64>* moves) {
  const SixTuple db = WhichDatabaseSlice();
  for (int direction = 0; direction < max_direction; ++direction) {
    const int to = move_offsets[from][direction];
    if (to < 0 || pieces[to] != Empty) {
      continue;
    }
    if (!moves) {
      return true;
    }
    MovePiece(from, to);
    moves->push_back(MirrorIndex(db.Mirror()));
    MovePiece(to, from);
  }
  if (!moves) {
    return false;
  }
  return moves->size() > 0;
}

bool Board::GenerateCaptures(int from,
                             int max_direction,
                             set<SevenTuple>* captures) {
  bool found_jumps = false;
  const Piece moving_piece = pieces[from];
  for (int direction = 0; direction < max_direction; ++direction) {
    const int to = jump_offsets[from][direction];
    const int over = move_offsets[from][direction];
    const Piece captured_piece = pieces[over];
    if (to < 0 || pieces[to] != Empty || !IsWhite(captured_piece)) {
      continue;
    }
    if (!captures) {
      return true;
    }
    found_jumps = true;
    pieces[from] = Empty;
    pieces[over] = Empty;
    // This line promotes pawns to kings when they reach the last rank.
    pieces[to] = to < 28 ? moving_piece : BlackKing;
    if (!GenerateCaptures(to, max_direction, captures)) {
      captures->insert(MirrorIndex());
    }
    pieces[from] = moving_piece;
    pieces[over] = captured_piece;
    pieces[to] = Empty;
  }
  return found_jumps;
}

bool Board::PawnMoves(int from, vector<uint64>* moves) {
  return NonConversionMoves(from, 2, moves);
}

bool Board::KingMoves(int from, vector<uint64>* moves) {
  return NonConversionMoves(from, 4, moves);
}

bool Board::IsOnBoard(int x, int y) {
  return x >= 0 && y >= 0 && x < 8 && y < 8;
}

int Board::XYToSpaceNumber(int x, int y) {
  return (x / 2) + (y * 4);
}

void Board::SpaceNumberToXY(int space, int* x, int* y) {
  *y = space / 4;
  *x = 2 * (space % 4) + (*y % 2);
}

bool Board::operator==(const Board& other) const {
  for (int i = 0; i < 32; ++i) {
    if (pieces[i] != other.pieces[i]) {
      return false;
    }
  }
  return true;
}

bool Board::operator!=(const Board& other) const {
  return !(*this == other);
}

// This is the Board's << operator for stream-style output. It's a friend.
ostream& operator<<(ostream &out, const Board& board) {
    out << board.HumanReadable();
    return out;
}

string Board::HumanReadable() const {
  string res;
  for (int i = 0; i < 32; ++i) {
    if (i % 8 == 0) {
      res += "  ";
    }
    const int square = human_readable_ordering[i];
    const int value = (int)pieces[square];
    res += " " + string(1, human_readable_chars[value]) + " ";
    if (i % 4 == 3) {
      res += "\n";
    }
  }
  return res;
}
