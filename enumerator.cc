#include "board.h"
#include "combinator.h"
#include "enumerator.h"
#include "pawn-cache.h"
#include "six-tuple.h"
#include "std.h"

Enumerator::Enumerator(int nbk, int nwk, int nbp, int nwp, int rbp, int rwp)
  // Initializer list.
  : db(nbk, nwk, nbp, nwp, rbp, rwp),
    index(0),
    bp(4 * (rbp + 1), nbp),
    wp(NULL),
    bk(32 - nbp - nwp, nbk),
    wk(32 - nbp - nwp - nbk, nwk),
    pc(PawnCache::Get(SixTuple(nbk, nwk, nbp, nwp, rbp, rwp))),
    num_positions(pc.MaxWP() * bk.NumCombinations() * wk.NumCombinations()) {
  // The body of the function starts here.
  Deindex(0);
}

Enumerator::Enumerator(SixTuple db)
  // Initializer list.
  : db(db),
    index(0),
    bp(4 * (db.rbp + 1), db.nbp),
    wp(NULL),
    bk(32 - db.nbp - db.nwp, db.nbk),
    wk(32 - db.nbp - db.nwp - db.nbk, db.nwk),
    pc(PawnCache::Get(db)),
    num_positions(pc.MaxWP() * bk.NumCombinations() * wk.NumCombinations()) {
  // The body of the function starts here.
  Deindex(0);
}

Enumerator::~Enumerator() {
  if (wp) {
    delete wp;
  }
}

uint64 Enumerator::NumPositions() const {
  return num_positions;
}

bool Enumerator::Increment() {
  ++index;
  if (db.nwk > 0) {
    if (!wk.Increment(&wk_squares, &board)) {
      return false;
    }
    board.Clear(WhiteKing);
  }
  if (db.nbk > 0) {
    if (!bk.Increment(&bk_squares, &board)) {
      SetupWhiteKings();
      return false;
    }
    board.Clear(BlackKing);
  }
  if (db.nwp > 0) {
    if (!wp->Increment(&wp_squares, &board)) {
      if (wp->Index() < pc.NumWP(bp.Index())) {
        SetupBlackKings();
        SetupWhiteKings();
        return false;
      }
    }
    board.Clear(WhitePawn);
  }
  while (!bp.Increment(&bp_squares, &board) && bp.Index() < pc.MaxBP()) {
    if (pc.NumWP(bp.Index()) == 0) {
      continue;
    }
    SetupWhitePawns();
    SetupBlackKings();
    SetupWhiteKings();
    return false;
  }
  Deindex(0);
  return true;
}

bool Enumerator::Increment(uint64 count) {
  bool any_true = false;
  while (count > 0) {
    any_true = any_true || Increment();
    --count;
  }
  return any_true;
}

uint64 Enumerator::Index() const {
  return index;
}

void Enumerator::Deindex(uint64 new_index) {
  if (new_index >= num_positions) {
    throw "Index is too large.";
  }
  board.Clear();
  index = new_index;
  // Use division & modulus to find the indices for the kings.
  const uint64 wkr = new_index / wk.NumCombinations();
  const uint64 bkr = wkr / bk.NumCombinations();
  const uint64 wk_index = new_index - wk.NumCombinations() * wkr;
  const uint64 bk_index = wkr - bk.NumCombinations() * bkr;
  // Use binary search to find the pawn indices. This can't be done with
  // standard division & modulus because the white pawn cycles have different
  // lengths.
  uint64 bp_index;
  uint64 wp_index;
  pc.DecomposeIndex(bkr, &bp_index, &wp_index);
  // Black pawns.
  bp_squares.clear();
  for (int i = 4 * db.rbp + 3; i >= 0; --i) {
    bp_squares.push_back(i);
  }
  bp.Deindex(bp_index, BlackPawn, &bp_squares, &board);
  // White pawns.
  if (wp) {
    delete wp;
  }
  wp_squares.clear();
  for (int i = 4 * (7 - db.rwp); i < 32; ++i) {
    if (board.GetPiece(i) == Empty) {
      wp_squares.push_back(i);
    }
  }
  wp = new Combinator(wp_squares.size(), db.nwp);
  wp->Deindex(wp_index, WhitePawn, &wp_squares, &board);
  // Black kings.
  bk_squares.clear();
  for (int i = 0; i < 32; ++i) {
    if (board.GetPiece(i) == Empty) {
      bk_squares.push_back(i);
    }
  }
  bk.Deindex(bk_index, BlackKing, &bk_squares, &board);
  // White kings.
  wk_squares.clear();
  for (int i = 0; i < 32; ++i) {
    if (board.GetPiece(i) == Empty) {
      wk_squares.push_back(i);
    }
  }
  wk.Deindex(wk_index, WhiteKing, &wk_squares, &board);
}

const Board& Enumerator::GetBoard() const {
  return board;
}

bool Enumerator::operator==(const Enumerator& other) const {
  if (db != other.db) {
    // The other iterator is iterating a different database slice.
    return false;
  }
  if (board != other.board) {
    // The checkers on the other board are arranged differently.
    return false;
  }
  if (!wp || !other.wp) {
    // White pawns are not initialized. This shouldn't happen.
    return false;
  }
  if (bp != other.bp || *wp != *other.wp || bk != other.bk || wk != other.wk) {
    // One of the four iterators is at a different position.
    return false;
  }
  // If we get here, then the two Enumerators are equal.
  return true;
}

bool Enumerator::operator!=(const Enumerator& other) const {
  return !(*this == other);
}

bool Enumerator::operator==(const Board& b) const {
  return board == b;
}

bool Enumerator::operator!=(const Board& b) const {
  return !(*this == b);
}

ostream& operator<<(ostream &out, const Enumerator& e) {
  out << "Enumerator(" << e.db << ") index: " << e.Index() << endl
      << "bp: " << e.bp << endl << "wp: " << (*e.wp) << endl
      << "bk: " << e.bk << endl << "wk: " << e.wk << endl << e.board;
  return out;
}

void Enumerator::SetupWhitePawns() {
  if (wp && db.nwp == 0) {
    return;
  }
  if (wp) {
    delete wp;
  }
  wp_squares.clear();
  for (int i = 4 * (7 - db.rwp); i < 32; ++i) {
    if (board.GetPiece(i) == Empty) {
      wp_squares.push_back(i);
    }
  }
  wp = new Combinator(wp_squares.size(), db.nwp);
  for (int i = 0; i < db.nwp; ++i) {
    board.SetPiece(wp_squares[i], WhitePawn);
  }
}

void Enumerator::SetupBlackKings() {
  if (db.nbk == 0) {
    return;
  }
  bk.Reset();
  bk_squares.clear();
  for (int i = 0; i < 32; ++i) {
    if (board.GetPiece(i) == Empty) {
      bk_squares.push_back(i);
    }
  }
  for (int i = 0; i < db.nbk; ++i) {
    board.SetPiece(bk_squares[i], BlackKing);
  }
}

void Enumerator::SetupWhiteKings() {
  if (db.nwk == 0) {
    return;
  }
  wk.Reset();
  wk_squares.clear();
  for (int i = 0; i < 32; ++i) {
    if (board.GetPiece(i) == Empty) {
      wk_squares.push_back(i);
    }
  }
  for (int i = 0; i < db.nwk; ++i) {
    board.SetPiece(wk_squares[i], WhiteKing);
  }
}
