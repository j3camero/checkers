#include "bit-string.h"
#include "catch.hpp"

#include <iostream>

TEST_CASE("Empty constructor", "[Bitstring]") {
  Bitstring b;
  REQUIRE(b.Size() == 0);
}

TEST_CASE("One bit constructor", "[Bitstring]") {
  Bitstring b(true);
  REQUIRE(b.Size() == 1);
  REQUIRE(b.Get(0) == true);
}

TEST_CASE("Construct by size", "[Bitstring]") {
  uint64 size = 99;
  Bitstring b(size);
  REQUIRE(b.Size() == 99);
  REQUIRE(b.Get(0) == false);
  REQUIRE(b.Get(98) == false);
}

TEST_CASE("Copy constructor", "[Bitstring]") {
  Bitstring b;
  b.Append(true);
  b.Append(false);
  b.Append(true);
  b.Append(false);
  b.Append(true);
  // Copy the bit string.
  Bitstring c(b);
  REQUIRE(c.Size() == 5);
  REQUIRE(c.Get(0) == true);
  REQUIRE(c.Get(1) == false);
  REQUIRE(c.Get(4) == true);
  // Check that the copy is a real deep copy.
  c.Set(1, true);
  REQUIRE(c.Get(1) == true);
  REQUIRE(b.Get(1) == false);
}

TEST_CASE("String constructor", "[Bitstring]") {
  Bitstring b("01010");
  REQUIRE(b.Size() == 5);
  REQUIRE(b.Get(0) == false);
  REQUIRE(b.Get(1) == true);
  REQUIRE(b.Get(4) == false);

  Bitstring z("");
  REQUIRE(z.Size() == 0);
}

TEST_CASE("Set and Get", "[Bitstring]") {
  uint64 size = 99;
  Bitstring b(size);
  b.Set(0, true);
  b.Set(7, true);
  b.Set(98, true);
  REQUIRE(b.Get(0) == true);
  REQUIRE(b.Get(1) == false);
  REQUIRE(b.Get(7) == true);
  REQUIRE(b.Get(97) == false);
  REQUIRE(b.Get(98) == true);
  // Check that bits are re-assignable more than once.
  b.Set(7, false);
  REQUIRE(b.Get(7) == false);
}

TEST_CASE("Resize", "[Bitstring]") {
  Bitstring b;
  b.Resize(9001);
  b.Set(0, true);
  b.Set(8000, true);
  b.Set(9000, true);
  REQUIRE(b.Size() == 9001);
  REQUIRE(b.Get(0) == true);
  REQUIRE(b.Get(1) == false);
  REQUIRE(b.Get(9000) == true);
  b.Resize(8001);
  REQUIRE(b.Get(8000) == true);
  REQUIRE(b.Get(7999) == false);
}

TEST_CASE("Big Bitstring (1MB+)", "[Bitstring]") {
  uint64 size = 12 * 1024 * 1024;
  Bitstring b(size);
  b.Set(0, true);
  b.Set(size - 1, true);
  REQUIRE(b.Size() == size);
  REQUIRE(b.Get(0) == true);
  REQUIRE(b.Get(1) == false);
  REQUIRE(b.Get(size - 2) == false);
  REQUIRE(b.Get(size - 1) == true);
}

TEST_CASE("Append one bit at a time", "[Bitstring]") {
  Bitstring b;
  b.Append(true);
  REQUIRE(b.Size() == 1);
  REQUIRE(b.Get(0) == true);
  b.Append(false);
  REQUIRE(b.Size() == 2);
  REQUIRE(b.Get(1) == false);
  b.Append(true);
  REQUIRE(b.Size() == 3);
  REQUIRE(b.Get(0) == true);
  REQUIRE(b.Get(1) == false);
  REQUIRE(b.Get(2) == true);
}

TEST_CASE("Append a bitstring to another", "[Bitstring]") {
  Bitstring a;
  a.Append(true);
  a.Append(false);
  Bitstring b;
  b.Append(false);
  b.Append(true);
  b.Append(false);
  a.Append(b);
  REQUIRE(a.Size() == 5);
  REQUIRE(a.Get(0) == true);
  REQUIRE(a.Get(1) == false);
  REQUIRE(a.Get(2) == false);
  REQUIRE(a.Get(3) == true);
  REQUIRE(a.Get(4) == false);
}

TEST_CASE("Bitstring as number", "[Bitstring]") {
  Bitstring b;
  REQUIRE(b.ToUInt64() == 0);
  b.Append(false);
  REQUIRE(b.ToUInt64() == 0);
  b.Append(true);
  REQUIRE(b.ToUInt64() == 2);
  b.Append(false);
  REQUIRE(b.ToUInt64() == 2);
  b.Append(true);
  REQUIRE(b.ToUInt64() == 10);
}

TEST_CASE("Compare zero runs", "[Bitstring]") {
  REQUIRE(Bitstring() < Bitstring(uint64(1)));
  REQUIRE(Bitstring(uint64(3)) < Bitstring(uint64(5)));
  REQUIRE(!(Bitstring() < Bitstring()));
  REQUIRE(!(Bitstring(uint64(1)) < Bitstring()));
  REQUIRE(!(Bitstring(uint64(7)) < Bitstring(uint64(4))));
}

TEST_CASE("Compare strings", "[Bitstring]") {
  REQUIRE(Bitstring() < Bitstring(false));
  REQUIRE(Bitstring() < Bitstring(true));
  REQUIRE(Bitstring(false) < Bitstring(true));
}
