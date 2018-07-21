#include "board.h"

#include "combinator.h"
#include "pawn-cache.h"
#include "seven-tuple.h"
#include "six-tuple.h"

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

Board::Board() {
  Clear();
}

Board::Board(const std::string& s) {
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
  int avail = 4 * db.rbp + 3;
  uint64 c = 0;
  for (int i = avail; i >= 0; --i) {
    if (pieces[i] == BlackPawn) {
      --avail;
      bp_index *= avail;
      bp_index += c;
      c = 0;
    } else {
      c++;
    }
  }
  // Calculate white pawn sub-index.
  uint64 wp_index = 0;
  avail = 4 * (db.rwp + 1);
  c = 0;
  for (int i = 4 * (7 - db.rwp); i < 32; ++i) {
    switch (pieces[i]) {
      case BlackPawn:
        break;
      case WhitePawn:
        --avail;
        wp_index *= avail;
        wp_index += c;
        c = 0;
        break;
      default:
        c++;
        break;
    };
  }
  // Calculate black king sub-index.
  uint64 bk_index = 0;
  avail = 32 - db.nbp - db.nwp;
  c = 0;
  for (int i = 0; i < 32; ++i) {
    switch (pieces[i]) {
      case BlackPawn:
      case WhitePawn:
        break;
      case BlackKing:
        --avail;
        bk_index *= avail;
        bk_index += c;
        c = 0;
        break;
      default:
        c++;
        break;
    };
  }
  // Calculate white king sub-index.
  uint64 wk_index = 0;
  c = 0;
  for (int i = 0; i < 32; ++i) {
    switch (pieces[i]) {
      case WhiteKing:
        --avail;
        wk_index *= avail;
        wk_index += c;
        c = 0;
        break;
      case Empty:
        c++;
        break;
      default:
        break;
    };
  }
  // Calculate a combined overall index from the separate sub-indices.
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
std::ostream& operator<<(std::ostream &out, const Board& board) {
    out << board.HumanReadable();
    return out;
}

std::string Board::HumanReadable() const {
  std::string res;
  for (int i = 0; i < 32; ++i) {
    if (i % 8 == 0) {
      res += "  ";
    }
    const int square = human_readable_ordering[i];
    const int value = (int)pieces[square];
    res += " " + std::string(1, human_readable_chars[value]) + " ";
    if (i % 4 == 3) {
      res += "\n";
    }
  }
  return res;
}
