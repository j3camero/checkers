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

// Disabled because it takes several seconds to run.
// TEST_CASE("Cycle of 465M+", "[Enumerator]") {
//   Enumerator e(3, 2, 1, 2, 6, 6);
//   REQUIRE(e == Board("   -   -   -   - "
//                      " -   -   -   b   "
//                      "   -   -   -   - "
//                      " -   -   -   -   "
//                      "   -   -   -   - "
//                      " -   -   -   -   "
//                      "   w   w   W   - "
//                      " B   B   B   W   "));
//   REQUIRE(!e.Increment(465519599));
//   REQUIRE(e == Board("   B   B   B   w "
//                      " b   -   W   W   "
//                      "   -   -   -   - "
//                      " -   -   -   -   "
//                      "   -   -   -   - "
//                      " -   -   -   -   "
//                      "   -   -   -   w "
//                      " -   -   -   -   "));
//   REQUIRE(e.Increment());
//   REQUIRE(e == Board("   -   -   -   - "
//                      " -   -   -   b   "
//                      "   -   -   -   - "
//                      " -   -   -   -   "
//                      "   -   -   -   - "
//                      " -   -   -   -   "
//                      "   w   w   W   - "
//                      " B   B   B   W   "));
//   REQUIRE(!e.Increment());
// }
