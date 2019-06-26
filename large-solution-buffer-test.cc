#include "catch.hpp"
#include "large-solution-buffer.h"

TEST_CASE("LargeSolutionBuffer small", "[LargeSolutionBuffer]") {
  LargeSolutionBuffer b(7);
  REQUIRE(b.Length() == 7);
  REQUIRE(b.Get(0) == Unknown);
  REQUIRE(b.Get(6) == Unknown);
  b.Set(0, Win);
  b.Set(1, Loss);
  b.Set(6, Draw);
  REQUIRE(b.Get(0) == Win);
  REQUIRE(b.Get(1) == Loss);
  REQUIRE(b.Get(2) == Unknown);
  REQUIRE(b.Get(5) == Unknown);
  REQUIRE(b.Get(6) == Draw);
}

TEST_CASE("LargeSolutionBuffer medium", "[LargeSolutionBuffer]") {
  LargeSolutionBuffer b(99);
  REQUIRE(b.Length() == 99);
  REQUIRE(b.Get(0) == Unknown);
  REQUIRE(b.Get(98) == Unknown);
  b.Set(0, Win);
  b.Set(1, Loss);
  b.Set(98, Draw);
  REQUIRE(b.Get(0) == Win);
  REQUIRE(b.Get(1) == Loss);
  REQUIRE(b.Get(2) == Unknown);
  REQUIRE(b.Get(97) == Unknown);
  REQUIRE(b.Get(98) == Draw);
}

TEST_CASE("LargeSolutionBuffer big example", "[LargeSolutionBuffer]") {
  LargeSolutionBuffer b(654321);
  REQUIRE(b.Length() == 654321);
  REQUIRE(b.Get(0) == Unknown);
  REQUIRE(b.Get(654320) == Unknown);
  b.Set(0, Win);
  b.Set(1, Loss);
  b.Set(654320, Draw);
  REQUIRE(b.Get(0) == Win);
  REQUIRE(b.Get(1) == Loss);
  REQUIRE(b.Get(2) == Unknown);
  REQUIRE(b.Get(654319) == Unknown);
  REQUIRE(b.Get(654320) == Draw);
}

TEST_CASE("LargeSolutionBuffer huge example", "[LargeSolutionBuffer]") {
  LargeSolutionBuffer b(999999999);
  REQUIRE(b.Length() == 999999999);
  REQUIRE(b.Get(0) == Unknown);
  REQUIRE(b.Get(999999998) == Unknown);
  b.Set(0, Win);
  b.Set(1, Loss);
  b.Set(100000007, Draw);
  b.Set(999999998, Draw);
  REQUIRE(b.Get(0) == Win);
  REQUIRE(b.Get(1) == Loss);
  REQUIRE(b.Get(2) == Unknown);
  REQUIRE(b.Get(100000006) == Unknown);
  REQUIRE(b.Get(100000007) == Draw);
  REQUIRE(b.Get(100000008) == Unknown);
  REQUIRE(b.Get(999999997) == Unknown);
  REQUIRE(b.Get(999999998) == Draw);
}

TEST_CASE("LargeSolutionBuffer typedef / alias", "[LargeSolutionBuffer]") {
  // Optional shorter type name to use for code readability.
  SolutionBuffer b(7);
  REQUIRE(b.Length() == 7);
}
