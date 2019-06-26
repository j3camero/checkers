#include "catch.hpp"
#include "medium-solution-buffer.h"
#include "std.h"

TEST_CASE("Small buffer", "[MediumSolutionBuffer]") {
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

TEST_CASE("Medium buffer", "[MediumSolutionBuffer]") {
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

TEST_CASE("Large buffer", "[MediumSolutionBuffer]") {
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

TEST_CASE("Write to disk and read back", "[MediumSolutionBuffer]") {
  MediumSolutionBuffer a(99);
  a.Set(0, Win);
  a.Set(1, Loss);
  a.Set(98, Draw);
  const string filename("test-solution-buffer.egd");
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
