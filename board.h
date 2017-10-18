#ifndef _BOARD_H_
#define _BOARD_H_

#include <string>

enum Piece {
  Empty = 0,
  WhiteKing = 1,
  BlackKing = 2,
  WhitePawn = 3,
  BlackPawn = 4
};

class Board {
 public:
  // Initialize an empty board.
  Board();

  // Initialize a board from a human-readable string.
  Board(const std::string& s);

  // Sets all the spaces to empty.
  void Clear();

  bool operator==(const Board& other) const;

  // Format the board as an easy-to-read string.
  std::string HumanReadable();

 private:
  Piece pieces[32];
};

#endif
