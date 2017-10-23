#include "catch.hpp"
#include "enumerator.h"

TEST_CASE("Construct small enumerator", "[Enumerator]") {
  Enumerator e(0, 0, 1, 1, 2, 2);
  REQUIRE(e.NumPositions() == 16);
}

TEST_CASE("Construct large enumerator", "[Enumerator]") {
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
