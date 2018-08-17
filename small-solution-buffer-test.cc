#include "catch.hpp"
#include "small-solution-buffer.h"

TEST_CASE("Get and set", "[SmallSolutionBuffer]") {
  SmallSolutionBuffer b;
  REQUIRE(b.Get(0) == Unknown);
  REQUIRE(b.Get(7) == Unknown);
  REQUIRE(b.Get(31) == Unknown);
  b.Set(0, Win);
  b.Set(1, Loss);
  b.Set(6, Draw);
  REQUIRE(b.Get(0) == Win);
  REQUIRE(b.Get(1) == Loss);
  REQUIRE(b.Get(2) == Unknown);
  REQUIRE(b.Get(5) == Unknown);
  REQUIRE(b.Get(6) == Draw);
}
