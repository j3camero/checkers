#include "catch.hpp"
#include "six-tuple.h"

TEST_CASE("SixTuple constructor", "[SixTuple]") {
  SixTuple s(3, 2, 1, 2, 4, 3);
  REQUIRE(s.nbk == 3);
  REQUIRE(s.nwk == 2);
  REQUIRE(s.nbp == 1);
  REQUIRE(s.nwp == 2);
  REQUIRE(s.rbp == 4);
  REQUIRE(s.rwp == 3);
}

TEST_CASE("SixTuple copy constructor", "[SixTuple]") {
  SixTuple s(3, 2, 1, 2, 4, 3);
  SixTuple t(s);
  REQUIRE(t.nbk == 3);
  REQUIRE(t.nwk == 2);
  REQUIRE(t.nbp == 1);
  REQUIRE(t.nwp == 2);
  REQUIRE(t.rbp == 4);
  REQUIRE(t.rwp == 3);
}

TEST_CASE("SixTuple mirror", "[SixTuple]") {
  SixTuple s(3, 2, 1, 2, 4, 3);
  SixTuple t = s.Mirror();
  // Black and white pieces should be reversed.
  REQUIRE(t.nbk == 2);
  REQUIRE(t.nwk == 3);
  REQUIRE(t.nbp == 2);
  REQUIRE(t.nwp == 1);
  REQUIRE(t.rbp == 3);
  REQUIRE(t.rwp == 4);
}

TEST_CASE("SixTuple equality operator", "[SixTuple]") {
  // Test that the == and != are internally consistent with each other.
  REQUIRE(SixTuple(2, 4, 3, 1, 4, 3) == SixTuple(2, 4, 3, 1, 4, 3));
  REQUIRE_FALSE(SixTuple(2, 4, 3, 1, 4, 3) == SixTuple(2, 4, 3, 2, 4, 3));
  REQUIRE_FALSE(SixTuple(2, 4, 3, 1, 4, 3) != SixTuple(2, 4, 3, 1, 4, 3));
  REQUIRE(SixTuple(2, 4, 3, 1, 4, 3) != SixTuple(2, 4, 3, 2, 4, 3));
  // Test the two extra counts that make the 4-tuple into the 6-tuple.
  REQUIRE(SixTuple(2, 4, 3, 1, 4, 3) != SixTuple(2, 4, 3, 1, 5, 3));
  REQUIRE(SixTuple(2, 4, 3, 1, 4, 3) != SixTuple(2, 4, 3, 1, 4, 2));
}
