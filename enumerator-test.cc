#include "catch.hpp"
#include "enumerator.h"

TEST_CASE("Number-of-positions calculation", "[Enumerator]") {
  // Sanity check against position counts from Lake 1994.
  Enumerator a(3, 2, 1, 2, 0, 0);
  REQUIRE(a.NumPositions() == 28501200);
  Enumerator b(3, 2, 1, 2, 2, 2);
  REQUIRE(b.NumPositions() == 180507600);
  Enumerator c(3, 2, 1, 2, 6, 6);
  REQUIRE(c.NumPositions() == 465519600);
  Enumerator d(3, 2, 1, 2, 2, 5);
  REQUIRE(d.NumPositions() == 299262600);
  Enumerator e(3, 2, 1, 2, 5, 2);
  REQUIRE(e.NumPositions() == 128255400);
  Enumerator f(3, 2, 1, 2, 4, 5);
  REQUIRE(f.NumPositions() == 389516400);
  Enumerator g(3, 2, 1, 2, 5, 4);
  REQUIRE(g.NumPositions() == 313513200);
}

TEST_CASE("Two pawns", "[Enumerator]") {
  Enumerator e(0, 0, 1, 1, 2, 2);
  REQUIRE(e.NumPositions() == 16);
  REQUIRE(e == Board("   -   -   -   - "
                     " -   -   -   -   "
                     "   w   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   b   "
                     "   -   -   -   - "
                     " -   -   -   -   "));
  e.Increment();
  REQUIRE(e == Board("   -   -   -   - "
                     " -   -   -   -   "
                     "   -   w   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   b   "
                     "   -   -   -   - "
                     " -   -   -   -   "));
  e.Increment(2);
  REQUIRE(e == Board("   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   w "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   b   "
                     "   -   -   -   - "
                     " -   -   -   -   "));
  e.Increment();
  REQUIRE(e == Board("   -   -   -   - "
                     " -   -   -   -   "
                     "   w   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   b   -   "
                     "   -   -   -   - "
                     " -   -   -   -   "));
  e.Increment(11);
  REQUIRE(e == Board("   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   w "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " b   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   -   "));
  REQUIRE(e.Increment());
  REQUIRE(e == Board("   -   -   -   - "
                     " -   -   -   -   "
                     "   w   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   b   "
                     "   -   -   -   - "
                     " -   -   -   -   "));
  REQUIRE(!e.Increment());
}

TEST_CASE("Two pawns sharing the same row", "[Enumerator]") {
  Enumerator e(0, 0, 1, 1, 4, 3);
  REQUIRE(e.NumPositions() == 12);
  REQUIRE(e == Board("   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " w   -   -   b   "
                     "   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   -   "));
  e.Increment();
  REQUIRE(e == Board("   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   w   -   b   "
                     "   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   -   "));
  e.Increment(2);
  REQUIRE(e == Board("   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " w   -   b   -   "
                     "   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   -   "));
  e.Increment();
  REQUIRE(e == Board("   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   w   b   -   "
                     "   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   -   "));
  e.Increment(7);
  REQUIRE(e == Board("   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " b   -   -   w   "
                     "   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   -   "));
  REQUIRE(e.Increment());
  REQUIRE(e == Board("   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " w   -   -   b   "
                     "   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   -   "));
  REQUIRE(!e.Increment());
}

TEST_CASE("Two kings", "[Enumerator]") {
  Enumerator e(1, 1, 0, 0, 0, 0);
  REQUIRE(e.NumPositions() == 992);
  REQUIRE(e == Board("   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " B   W   -   -   "));
  REQUIRE(!e.Increment(991));
  REQUIRE(e == Board("   -   -   W   B "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   -   "));
  REQUIRE(e.Increment());
  REQUIRE(e == Board("   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " B   W   -   -   "));
  REQUIRE(!e.Increment());
}

TEST_CASE("4 pieces: one of each piece type", "[Enumerator]") {
  Enumerator e(1, 1, 1, 1, 2, 2);
  REQUIRE(e.NumPositions() == 13920);
  REQUIRE(e == Board("   -   -   -   - "
                     " -   -   -   -   "
                     "   w   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   b   "
                     "   -   -   -   - "
                     " B   W   -   -   "));
  REQUIRE(!e.Increment(13919));
  REQUIRE(e == Board("   -   -   W   B "
                     " -   -   -   -   "
                     "   -   -   -   w "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " b   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   -   "));
  REQUIRE(e.Increment());
  REQUIRE(e == Board("   -   -   -   - "
                     " -   -   -   -   "
                     "   w   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   b   "
                     "   -   -   -   - "
                     " B   W   -   -   "));
  REQUIRE(!e.Increment());
}

TEST_CASE("Cycle of 28M+", "[Enumerator]") {
  // Even larger benchmarks can be done using enumerator-benchmark.cc.
  Enumerator e(3, 2, 1, 2, 0, 0);
  REQUIRE(e == Board("   w   w   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   -   "
                     "   W   W   -   - "
                     " B   B   B   b   "));
  REQUIRE(!e.Increment(28501199));
  REQUIRE(e == Board("   B   B   w   w "
                     " -   W   W   B   "
                     "   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " b   -   -   -   "));
  REQUIRE(e.Increment());
  REQUIRE(e == Board("   w   w   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   -   "
                     "   W   W   -   - "
                     " B   B   B   b   "));
  REQUIRE(!e.Increment());
}

TEST_CASE("Deindex zero with small enumerator", "[Enumerator]") {
  Enumerator e(0, 0, 1, 1, 0, 0);
  e.Deindex(0);
  REQUIRE(e == Board("   w   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   b   "));
}

TEST_CASE("Deindex zero with big enumerator", "[Enumerator]") {
  Enumerator e(1, 2, 3, 2, 4, 3);
  e.Deindex(0);
  REQUIRE(e == Board("   -   -   -   - "
                     " -   -   -   -   "
                     "   w   -   -   - "
                     " w   b   b   b   "
                     "   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " B   W   W   -   "));
}

TEST_CASE("Deindex compare to increment, small", "[Enumerator]") {
  Enumerator a(0, 0, 1, 1, 2, 2);  // This one gets deindexed.
  Enumerator b(0, 0, 1, 1, 2, 2);  // This one gets incremented.
  for (uint64 i = 0; i < b.NumPositions(); ++i) {
    a.Deindex(i);
    REQUIRE(a == b);
    b.Increment();
  }
}

TEST_CASE("Deindex compare to increment, all 4 piece types", "[Enumerator]") {
  Enumerator a(1, 1, 1, 1, 4, 3);  // This one gets deindexed.
  Enumerator b(1, 1, 1, 1, 4, 3);  // This one gets incremented.
  const uint64 step = 499;
  for (uint64 i = 0; i < b.NumPositions(); i += step) {
    a.Deindex(i);
    REQUIRE(a == b);
    b.Increment(step);
  }
}

TEST_CASE("Deindex compare to increment, 28M+ loop", "[Enumerator]") {
  Enumerator a(3, 2, 1, 2, 0, 0);  // This one gets deindexed.
  Enumerator b(3, 2, 1, 2, 0, 0);  // This one gets incremented.
  const uint64 step = 1000007;
  for (uint64 i = 0; i < b.NumPositions(); i += step) {
    a.Deindex(i);
    REQUIRE(a == b);
    b.Increment(step);
  }
}
