#ifndef _PIECE_COUNT_H_
#define _PIECE_COUNT_H_

#include <fstream>

// A 4-tuple of integers. One for each piece type.
class PieceCount {
 public:
  // Constructor.
  PieceCount(int nbk, int nwk, int nbp, int nwp);

  // Copy constructor.
  PieceCount(const PieceCount& p);

  // Reverses the colors.
  PieceCount Mirror();

  // Operators.
  bool operator==(const PieceCount& other) const;
  bool operator!=(const PieceCount& other) const;
  bool operator<(const PieceCount& other) const;

  // Outputs as a 4-character string of digits with no newline, in the order:
  // "nbk nwk nbp nwp" as described in Lake (1994).
  friend std::ostream& operator<<(std::ostream &out, const PieceCount& e);

  // These variables are exposed as public. The usual OOP convention is to have
  // these be private and expose get and set functions instead. However, these
  // exact names in all-lowercase are strong conventions from the paper.
  int nbk;  // Number of black kings.
  int nwk;  // Number of white kings.
  int nbp;  // Number of black pawns.
  int nwp;  // Number of white pawns.
};

#endif
