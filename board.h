#ifndef _BOARD_H_
#define _BOARD_H_

#include <iostream>
#include <string>

#include "seven-tuple.h"
#include "six-tuple.h"

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

  // Determine which database slice this board position belongs to.
  SixTuple WhichDatabaseSlice() const;

  // Determine the index of this board position. The result is not defined if
  // the provided db does not match WhichDatabaseSlice(). Use this function
  // only if the caller is sure they know the correct SixTuple to use. If not,
  // use the overload of Index() that takes no arguments instead.
  uint64 Index(const SixTuple& db) const;

  // Determine the index of this board position, including which database slice.
  // Returns a SevenTuple containing which DB slice and which index.
  SevenTuple Index() const;

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
