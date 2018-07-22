#include "seven-tuple.h"

SevenTuple::SevenTuple(const SixTuple& db, uint64 index)
  : db(db), index(index) {
  // Body is empty because of initializer lists.
}

SevenTuple::SevenTuple(const SevenTuple& s)
  : db(s.db), index(s.index) {
  // Body is empty because of initializer lists.
}

const SixTuple& SevenTuple::GetDB() const {
  return db;
}

uint64 SevenTuple::GetIndex() const {
  return index;
}

bool SevenTuple::operator==(const SevenTuple& other) const {
  return db == other.db && index == other.index;
}

bool SevenTuple::operator!=(const SevenTuple& other) const {
  return !(*this == other);
}

std::ostream& operator<<(std::ostream &out, const SevenTuple& s) {
  out << s.db << ":" << s.index;
  return out;
}
