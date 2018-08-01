#include "catch.hpp"
#include "pawn-cache.h"

TEST_CASE("Easy case: two non-overlapping pawns", "[PawnCache]") {
  const PawnCache& pc = PawnCache::Get(SixTuple(0, 0, 1, 1, 2, 2));
  REQUIRE(pc.MaxBP() == 4);
  REQUIRE(pc.NumWP(0) == 4);
  REQUIRE(pc.NumWP(1) == 4);
  REQUIRE(pc.NumWP(2) == 4);
  REQUIRE(pc.NumWP(3) == 4);
  REQUIRE(pc.SumWP(0) == 0);
  REQUIRE(pc.SumWP(1) == 4);
  REQUIRE(pc.SumWP(2) == 8);
  REQUIRE(pc.SumWP(3) == 12);
  REQUIRE(pc.MaxWP() == 16);
}

TEST_CASE("Medium case: two overlapping pawns", "[PawnCache]") {
  const PawnCache& pc = PawnCache::Get(SixTuple(0, 0, 1, 1, 4, 3));
  REQUIRE(pc.MaxBP() == 4);
  REQUIRE(pc.NumWP(0) == 3);
  REQUIRE(pc.NumWP(1) == 3);
  REQUIRE(pc.NumWP(2) == 3);
  REQUIRE(pc.NumWP(3) == 3);
  REQUIRE(pc.SumWP(0) == 0);
  REQUIRE(pc.SumWP(1) == 3);
  REQUIRE(pc.SumWP(2) == 6);
  REQUIRE(pc.SumWP(3) == 9);
  REQUIRE(pc.MaxWP() == 12);
}

TEST_CASE("Hard case: four pawns", "[PawnCache]") {
  const PawnCache& pc = PawnCache::Get(SixTuple(0, 0, 2, 2, 4, 3));
  REQUIRE(pc.MaxBP() == 70);
  REQUIRE(pc.NumWP(0) == 25);
  REQUIRE(pc.NumWP(1) == 25);
  REQUIRE(pc.NumWP(2) == 25);
  // As the second black pawn leaves the leading rank, more space should open
  // up for white pawns.
  REQUIRE(pc.NumWP(3) == 39);
  // The last position should also have only one black pawn on the leading rank.
  REQUIRE(pc.NumWP(69) == 39);
}

TEST_CASE("Check for true re-use of pawn cache records", "[PawnCache]") {
  const PawnCache& a = PawnCache::Get(SixTuple(0, 0, 7, 2, 6, 6));
  const PawnCache& b = PawnCache::Get(SixTuple(3, 3, 3, 3, 3, 3));
  // This record should be the same as 'a'. It only differs in the number of
  // kings, which are ignored by the pawn cache.
  const PawnCache& c = PawnCache::Get(SixTuple(2, 4, 7, 2, 6, 6));
  REQUIRE(&a != &b);
  REQUIRE(&a == &c);
}

TEST_CASE("Four black pawns blocking the leading white rank.", "[PawnCache]") {
  const PawnCache& pc = PawnCache::Get(SixTuple(0, 0, 4, 1, 1, 6));
  REQUIRE(pc.MaxBP() == 69);
  REQUIRE(pc.NumWP(0) == 0);
  REQUIRE(pc.NumWP(1) == 1);
  REQUIRE(pc.NumWP(68) == 3);
}

TEST_CASE("Five black pawns blocking the leading white rank.", "[PawnCache]") {
  const PawnCache& pc = PawnCache::Get(SixTuple(0, 0, 5, 1, 1, 6));
  REQUIRE(pc.MaxBP() == 56);
  REQUIRE(pc.NumWP(0) == 0);
  REQUIRE(pc.NumWP(1) == 0);
  REQUIRE(pc.NumWP(2) == 0);
  REQUIRE(pc.NumWP(3) == 0);
  REQUIRE(pc.NumWP(4) == 1);
  REQUIRE(pc.SumWP(4) == 0);
  REQUIRE(pc.SumWP(5) == 1);
  REQUIRE(pc.NumWP(55) == 3);
}
