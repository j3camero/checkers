#ifndef _SIX_TUPLE_H_
#define _SIX_TUPLE_H_

#include <fstream>

#include "four-tuple.h"

// A 6-tuple of integers:
//   nbk - the number of black kings.
//   nwk - the number of white kings.
//   nbp - the number of black pawns.
//   nwp - the number of white pawns.
//   rbp - the rank of the leading black pawn [0-6].
//   rwp - the rank of the leading white pawn [0-6].
//
// A SixTuple refers uniquely to an endgame database slice.
// It doesn't store the endgame data itself, just names the database slice.
class SixTuple : public FourTuple {
 public:
  // Normal constructor.
  SixTuple(int nbk, int nwk, int nbp, int nwp, int rbp, int rwp);

  // Copy constructor.
  SixTuple(const SixTuple& d);

  // Default constructor. Yields 0000.00, which is not a real database slice.
  SixTuple();

  // Reverses the colors.
  SixTuple Mirror() const;

  // Parses a SixTuple in the format "XXXX.XX" in-place. Returns true on
  // success, false on error.
  bool Parse(const std::string& s);

  // Parses a SixTuple in the format "XXXX.XX". Throws an exception in case
  // of error.
  static SixTuple ParseOrDie(const std::string& s);

  // Operators.
  bool operator==(const SixTuple& other) const;
  bool operator!=(const SixTuple& other) const;

  // Outputs as a 7-character string of digits with no newline, in the order:
  // "nbk nwk nbp nwp . rbp rwp" as described in Lake (1994). ex: 3212.43
  friend std::ostream& operator<<(std::ostream &out, const SixTuple& e);

  // These variables are exposed as public. The usual OOP convention is to have
  // these be private and expose get and set functions instead. However, these
  // exact names in all-lowercase are strong conventions from the paper.
  int rbp;  // Rank of the leading black pawn (0-6).
  int rwp;  // Rank of the leading white pawn (0-6).
};

#endif
