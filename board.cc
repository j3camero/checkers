#include "board.h"

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

uint64 Board::Index(const SixTuple& db) {
  return 0;
}

SevenTuple Board::Index() {
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
