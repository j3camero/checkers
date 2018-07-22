#include "catch.hpp"
#include "seven-tuple.h"

TEST_CASE("SevenTuple constructor & getters", "[SevenTuple]") {
  SixTuple db(3, 2, 1, 2, 4, 3);
  SevenTuple s(db, 42);
  REQUIRE(s.GetDB() == db);
  REQUIRE(s.GetIndex() == 42);
}

TEST_CASE("SevenTuple copy constructor", "[SevenTuple]") {
  SixTuple db(3, 2, 1, 2, 4, 3);
  SevenTuple s(db, 42);
  SevenTuple t(s);
  REQUIRE(t.GetDB() == db);
  REQUIRE(t.GetIndex() == 42);
}

TEST_CASE("SevenTuple equality operator", "[SevenTuple]") {
  // Equal.
  REQUIRE(SevenTuple(SixTuple(2, 4, 3, 1, 4, 3), 42) ==
          SevenTuple(SixTuple(2, 4, 3, 1, 4, 3), 42));
  // db not equal.
  REQUIRE(SevenTuple(SixTuple(2, 4, 3, 1, 4, 3), 42) !=
          SevenTuple(SixTuple(2, 4, 3, 1, 4, 1), 42));
  // index not equal.
  REQUIRE(SevenTuple(SixTuple(2, 4, 3, 1, 4, 3), 42) !=
          SevenTuple(SixTuple(2, 4, 3, 1, 4, 3), 41));
}
