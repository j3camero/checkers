#ifndef _SEVEN_TUPLE_H_
#define _SEVEN_TUPLE_H_

#include "six-tuple.h"
#include "std.h"

// Stores the absolute index of a single checkers position. Has two parts:
//   - a SixTuple that identifies the endgame database slice, and
//   - the 64-bit index of the position within the endgame databse slice.
//
// This class is called SevenTuple because it contains a SixTuple plus one
// extra field.
class SevenTuple {
 public:
  // Constructor.
  SevenTuple(const SixTuple& db, uint64 index);

  // Copy constructor.
  SevenTuple(const SevenTuple& d);

  // Getters.
  const SixTuple& GetDB() const;
  uint64 GetIndex() const;

  // Operators.
  bool operator==(const SevenTuple& other) const;
  bool operator!=(const SevenTuple& other) const;
  bool operator<(const SevenTuple& other) const;

  // Outputs a string in the format db:index. ex: 3212.43:98765
  friend ostream& operator<<(ostream &out, const SevenTuple& e);

 private:
  SixTuple db;
  uint64 index;
};

#endif
