#ifndef _FOUR_TUPLE_H_
#define _FOUR_TUPLE_H_

#include "std.h"

// A 4-tuple of integers. One for each piece type.
//   nbk - the number of black kings.
//   nwk - the number of white kings.
//   nbp - the number of black pawns.
//   nwp - the number of white pawns.
class FourTuple {
 public:
  // Constructor.
  FourTuple(int nbk, int nwk, int nbp, int nwp);

  // Copy constructor.
  FourTuple(const FourTuple& p);

  // Default constructor. Yields (0,0,0,0) which is not a real database.
  FourTuple();

  // Reverses the colors.
  FourTuple Mirror() const;

  // Operators.
  bool operator==(const FourTuple& other) const;
  bool operator!=(const FourTuple& other) const;
  bool operator<(const FourTuple& other) const;

  // Outputs as a 4-character string of digits with no newline, in the order:
  // "nbk nwk nbp nwp" as described in Lake (1994).
  friend ostream& operator<<(ostream &out, const FourTuple& e);

  // These variables are exposed as public. The usual OOP convention is to have
  // these be private and expose get and set functions instead. However, these
  // exact names in all-lowercase are strong conventions from the paper.
  int nbk;  // Number of black kings.
  int nwk;  // Number of white kings.
  int nbp;  // Number of black pawns.
  int nwp;  // Number of white pawns.
};

#endif
