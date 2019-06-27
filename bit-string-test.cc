#include "bit-string.h"
#include "catch.hpp"

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
  Bitstring b("11010");
  REQUIRE(b.Size() == 5);
  REQUIRE(b.Get(0) == false);
  REQUIRE(b.Get(1) == true);
  REQUIRE(b.Get(4) == true);

  Bitstring z("");
  REQUIRE(z.Size() == 0);

  Bitstring one("1");
  REQUIRE(one.Size() == 1);
  REQUIRE(one.Get(0) == true);

  Bitstring a("T+-tF");
  REQUIRE(a.Size() == 5);
  REQUIRE(a == b);

  REQUIRE(Bitstring("--++-+-") == Bitstring("0011010"));
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

TEST_CASE("Set bits with default argument", "[Bitstring]") {
  uint64 size = 99;
  Bitstring b(size);
  b.Set(7);
  REQUIRE(b.Get(7) == true);
}

TEST_CASE("Clear and flip bits", "[Bitstring]") {
  uint64 size = 99;
  Bitstring b(size);
  b.Set(7);
  REQUIRE(b.Get(7) == true);
  b.Clear(7);
  REQUIRE(b.Get(7) == false);
  b.Flip(7);
  REQUIRE(b.Get(7) == true);
  b.Flip(7);
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
  REQUIRE(b == Bitstring("101"));
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
  REQUIRE(Bitstring("110") < Bitstring("111"));
  REQUIRE(Bitstring("011") < Bitstring("111"));
  REQUIRE(Bitstring("101011") < Bitstring("101111"));
  REQUIRE(Bitstring("11") < Bitstring("000"));
}

TEST_CASE("String equality", "[Bitstring]") {
  Bitstring b("011");
  REQUIRE(b == "011");
  REQUIRE(b != "100");
  REQUIRE(!(b == "100"));
  REQUIRE(!(b != "011"));
}

TEST_CASE("Addition operator", "[Bitstring]") {
  REQUIRE(Bitstring("0") + Bitstring("1") == Bitstring("10"));
  REQUIRE(Bitstring("011") + Bitstring("001") == Bitstring("001011"));
  Bitstring b("011");
  b += Bitstring("01");
  REQUIRE(b == Bitstring("01011"));
}

TEST_CASE("Increment", "[Bitstring]") {
  Bitstring b;
  b.Increment();
  REQUIRE(b == Bitstring("1"));
  b.Increment();
  REQUIRE(b == Bitstring("10"));
  b.Increment();
  REQUIRE(b == Bitstring("11"));
  b.Increment();
  REQUIRE(b == Bitstring("100"));
  b.Increment();
  REQUIRE(b == Bitstring("101"));
  b.Increment();
  REQUIRE(b == Bitstring("110"));
}

TEST_CASE("Bitstring StartsWith", "[Bitstring]") {
  // Binary numbers are right-to-left.
  REQUIRE(Bitstring("001").StartsWith(Bitstring("1")));
  REQUIRE(Bitstring("001").StartsWith(Bitstring("01")));
  REQUIRE(Bitstring("001").StartsWith(Bitstring("001")));
  // All strings should start with the empty string.
  REQUIRE(Bitstring("001").StartsWith(Bitstring("")));
  // The empty string starts with the empty string.
  REQUIRE(Bitstring("").StartsWith(Bitstring("")));
  // The empty string does not start with any other string. Only with itself.
  REQUIRE(!Bitstring("").StartsWith(Bitstring("1")));
  REQUIRE(!Bitstring("").StartsWith(Bitstring("01")));
  // Strings don't start with strings longer than themselves.
  REQUIRE(!Bitstring("01").StartsWith(Bitstring("101")));
  // Non-matching examples.
  REQUIRE(!Bitstring("10110010").StartsWith(Bitstring("1")));
  REQUIRE(!Bitstring("10110010").StartsWith(Bitstring("00")));
  REQUIRE(!Bitstring("10110010").StartsWith(Bitstring("11")));
  REQUIRE(!Bitstring("10110010").StartsWith(Bitstring("110")));
  // Long-ish successful matching example.
  Bitstring b("001111001110100111101010111110000111001111001101101000011010");
  Bitstring prefix("1001110100111101010111110000111001111001101101000011010");
  REQUIRE(b.StartsWith(prefix));
  // The long string also matches itself.
  REQUIRE(b.StartsWith(b));
  // For that matter, the prefix also matches itself.
  REQUIRE(prefix.StartsWith(prefix));
  // ...but the prefix does not start with b because b is longer.
  REQUIRE(!prefix.StartsWith(b));
}

TEST_CASE("Bitstring IsPrefixFree edge cases", "[Bitstring]") {
  vector<Bitstring> v;
  // An empty set of binary strings is prefix-free. I guess.
  REQUIRE(Bitstring::IsPrefixFree(v));
  v.push_back(Bitstring("101"));
  // Any 1 binary string is also prefix-free, by itself.
  REQUIRE(Bitstring::IsPrefixFree(v));
}

TEST_CASE("Bitstring IsPrefixFree 2 bit strings", "[Bitstring]") {
  // Two equal strings are not prefix-free.
  vector<Bitstring> same;
  same.push_back(Bitstring("10101"));
  same.push_back(Bitstring("10101"));
  REQUIRE(!Bitstring::IsPrefixFree(same));
  // Two strings are not prefix-free if one starts with the other.
  vector<Bitstring> not_prefix_free;
  not_prefix_free.push_back(Bitstring("10101"));
  not_prefix_free.push_back(Bitstring("01"));
  REQUIRE(!Bitstring::IsPrefixFree(not_prefix_free));
  // Two strings are prefix-free if neither starts with the other.
  vector<Bitstring> prefix_free;
  prefix_free.push_back(Bitstring("10101"));
  prefix_free.push_back(Bitstring("110"));
  REQUIRE(Bitstring::IsPrefixFree(prefix_free));
}

TEST_CASE("Bitstring IsPrefixFree 3 bit strings", "[Bitstring]") {
  vector<Bitstring> not_prefix_free;
  not_prefix_free.push_back(Bitstring("10101"));
  not_prefix_free.push_back(Bitstring("110"));
  not_prefix_free.push_back(Bitstring("0110"));
  REQUIRE(!Bitstring::IsPrefixFree(not_prefix_free));
  vector<Bitstring> prefix_free;
  prefix_free.push_back(Bitstring("0010100"));
  prefix_free.push_back(Bitstring("0010110"));
  prefix_free.push_back(Bitstring("00110"));
  prefix_free.push_back(Bitstring("1110"));
  REQUIRE(Bitstring::IsPrefixFree(prefix_free));
}

TEST_CASE("Bitstring IsPrefixFree big example", "[Bitstring]") {
  vector<Bitstring> b;
  b.push_back(Bitstring("1001101111011011100111111001010111011110111"));
  b.push_back(Bitstring("11011001001111110010111011010000011101001111"));
  b.push_back(Bitstring("100111101100000010110010100001011111100110"));
  b.push_back(Bitstring("111111101111110111101000001001010011000010010"));
  b.push_back(Bitstring("01101111011000010010101110111100110001110"));
  b.push_back(Bitstring("1001110101110101111100011110000111011101011011"));
  b.push_back(Bitstring("0111110111011001100011011100000101101001"));
  b.push_back(Bitstring("11010000101011100100001100110101111010100110100"));
  b.push_back(Bitstring("000011100011001101110110101011010100010"));
  REQUIRE(Bitstring::IsPrefixFree(b));
  b.push_back(Bitstring("01110101110101111100011110000111011101011011"));
  REQUIRE(!Bitstring::IsPrefixFree(b));
}
