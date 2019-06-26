#include "catch.hpp"
#include "solution.h"
#include "std.h"

TEST_CASE("SmallSolutionBuffer get & set", "[Solution]") {
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

TEST_CASE("MediumSolutionBuffer small-ish buffer", "[Solution]") {
  MediumSolutionBuffer b(7);
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

TEST_CASE(" MediumSolutionBuffer medium-ish buffer", "[Solution]") {
  MediumSolutionBuffer b(99);
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

TEST_CASE("MediumSolutionBuffer large buffer", "[Solution]") {
  MediumSolutionBuffer b(654321);
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

TEST_CASE("Write to disk and read back", "[Solution]") {
  MediumSolutionBuffer a(99);
  a.Set(0, Win);
  a.Set(1, Loss);
  a.Set(98, Draw);
  const string filename("test-solution-buffer.end");
  bool write_success = a.Write(filename);
  REQUIRE(write_success);
  MediumSolutionBuffer b(filename);
  REQUIRE(b.Length() == 99);
  REQUIRE(b.Get(0) == Win);
  REQUIRE(b.Get(1) == Loss);
  REQUIRE(b.Get(2) == Unknown);
  REQUIRE(b.Get(97) == Unknown);
  REQUIRE(b.Get(98) == Draw);
}

TEST_CASE("LargeSolutionBuffer small", "[Solution]") {
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

TEST_CASE("LargeSolutionBuffer medium", "[Solution]") {
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

TEST_CASE("LargeSolutionBuffer big example", "[Solution]") {
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

TEST_CASE("LargeSolutionBuffer huge example", "[Solution]") {
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

TEST_CASE("LargeSolutionBuffer typedef / alias", "[Solution]") {
  // Optional shorter type name to use for code readability.
  SolutionBuffer b(7);
  REQUIRE(b.Length() == 7);
}
