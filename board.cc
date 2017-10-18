#include "board.h"

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
  Clear();
}

void Board::Clear() {
  for (int i = 0; i < 32; ++i) {
    pieces[i] = Empty;
  }
}

bool Board::operator==(const Board& other) const {
  for (int i = 0; i < 32; ++i) {
    if (pieces[i] != other.pieces[i]) {
      return false;
    }
  }
  return true;
}

std::string Board::HumanReadable() {
  return "";
}
