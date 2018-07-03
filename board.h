#ifndef _BOARD_H_
#define _BOARD_H_

#include <iostream>
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

  Piece GetPiece(int index) const;
  void SetPiece(int index, Piece p);

  // Moves the value from square 'from' to square 'to'. No check is made that
  // 'to' is empty. If not, then the piece would be overwritten.
  void MovePiece(int from, int to);

  // Sets all the spaces to empty.
  void Clear();

  // Removes all pices of a certain type from the board.
  void Clear(Piece p);

  // Equality operator.
  bool operator==(const Board& other) const;
  bool operator!=(const Board& other) const;

  // Format the board as an easy-to-read string.
  std::string HumanReadable() const;

  // Support stream-style printing of the board using the << operator.
  friend std::ostream& operator<<(std::ostream &out, const Board& board);

 private:
  Piece pieces[32];
};

#endif
