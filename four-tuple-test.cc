#include "catch.hpp"
#include "four-tuple.h"

TEST_CASE("FourTuple constructor", "[FourTuple]") {
  FourTuple p(3, 2, 1, 2);
  REQUIRE(p.nbk == 3);
  REQUIRE(p.nwk == 2);
  REQUIRE(p.nbp == 1);
  REQUIRE(p.nwp == 2);
}

TEST_CASE("FourTuple copy constructor", "[FourTuple]") {
  FourTuple a(3, 2, 1, 2);
  FourTuple b(a);
  REQUIRE(b.nbk == 3);
  REQUIRE(b.nwk == 2);
  REQUIRE(b.nbp == 1);
  REQUIRE(b.nwp == 2);
}

TEST_CASE("FourTuple mirror", "[FourTuple]") {
  FourTuple a(3, 2, 1, 2);
  FourTuple b = a.Mirror();
  // Black and white pieces should be reversed.
  REQUIRE(b.nbk == 2);
  REQUIRE(b.nwk == 3);
  REQUIRE(b.nbp == 2);
  REQUIRE(b.nwp == 1);
}

TEST_CASE("FourTuple equality operator", "[FourTuple]") {
  REQUIRE(FourTuple(2, 4, 3, 1) == FourTuple(2, 4, 3, 1));
  REQUIRE_FALSE(FourTuple(2, 4, 3, 1) == FourTuple(2, 4, 3, 2));
  REQUIRE_FALSE(FourTuple(2, 4, 3, 1) != FourTuple(2, 4, 3, 1));
  REQUIRE(FourTuple(2, 4, 3, 1) != FourTuple(2, 4, 3, 2));
}

TEST_CASE("FourTuple comparison operator", "[FourTuple]") {
  REQUIRE(FourTuple(2, 4, 3, 1) < FourTuple(2, 4, 3, 2));
  REQUIRE(FourTuple(1, 4, 3, 1) < FourTuple(2, 4, 3, 1));
  REQUIRE(FourTuple(2, 4, 2, 2) < FourTuple(2, 4, 3, 2));
  REQUIRE_FALSE(FourTuple(2, 4, 3, 1) < FourTuple(2, 4, 3, 1));
  REQUIRE(FourTuple(1, 4, 3, 1) < FourTuple(2, 4, 3, 1));
}
