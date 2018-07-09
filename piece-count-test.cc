#include "catch.hpp"
#include "piece-count.h"

TEST_CASE("PieceCount constructor", "[PieceCount]") {
  PieceCount p(3, 2, 1, 2);
  REQUIRE(p.nbk == 3);
  REQUIRE(p.nwk == 2);
  REQUIRE(p.nbp == 1);
  REQUIRE(p.nwp == 2);
}

TEST_CASE("PieceCount copy constructor", "[PieceCount]") {
  PieceCount a(3, 2, 1, 2);
  PieceCount b(a);
  REQUIRE(b.nbk == 3);
  REQUIRE(b.nwk == 2);
  REQUIRE(b.nbp == 1);
  REQUIRE(b.nwp == 2);
}

TEST_CASE("PieceCount mirror", "[PieceCount]") {
  PieceCount a(3, 2, 1, 2);
  PieceCount b = a.Mirror();
  // Black and white pieces should be reversed.
  REQUIRE(b.nbk == 2);
  REQUIRE(b.nwk == 3);
  REQUIRE(b.nbp == 2);
  REQUIRE(b.nwp == 1);
}

TEST_CASE("PieceCount equality operator", "[PieceCount]") {
  REQUIRE(PieceCount(2, 4, 3, 1) == PieceCount(2, 4, 3, 1));
  REQUIRE_FALSE(PieceCount(2, 4, 3, 1) == PieceCount(2, 4, 3, 2));
  REQUIRE_FALSE(PieceCount(2, 4, 3, 1) != PieceCount(2, 4, 3, 1));
  REQUIRE(PieceCount(2, 4, 3, 1) != PieceCount(2, 4, 3, 2));
}

TEST_CASE("PieceCount comparison operator", "[PieceCount]") {
  REQUIRE(PieceCount(2, 4, 3, 1) < PieceCount(2, 4, 3, 2));
  REQUIRE(PieceCount(1, 4, 3, 1) < PieceCount(2, 4, 3, 1));
  REQUIRE(PieceCount(2, 4, 2, 2) < PieceCount(2, 4, 3, 2));
  REQUIRE_FALSE(PieceCount(2, 4, 3, 1) < PieceCount(2, 4, 3, 1));
  REQUIRE(PieceCount(1, 4, 3, 1) < PieceCount(2, 4, 3, 1));
}
