#include "bit-string.h"
#include "catch.hpp"
#include "huffman.h"
#include "std.h"
#include "types.h"

TEST_CASE("Canonical Huffman codes, minimal cases", "[Huffman]") {
  vector<int> bit_count;
  const vector<Bitstring> zero = CanonicalHuffmanCode(bit_count);
  REQUIRE(zero.size() == 0);
  bit_count.push_back(1);
  const vector<Bitstring> one = CanonicalHuffmanCode(bit_count);
  REQUIRE(one.size() == 1);
  REQUIRE(one[0] == Bitstring("0"));
}

TEST_CASE("Canonical Huffman codes, small example", "[Huffman]") {
  vector<int> bit_count;
  bit_count.push_back(1);
  bit_count.push_back(2);
  bit_count.push_back(3);
  bit_count.push_back(3);
  const vector<Bitstring> code = CanonicalHuffmanCode(bit_count);
  REQUIRE(code[0] == Bitstring("0"));
  REQUIRE(code[1] == Bitstring("10"));
  REQUIRE(code[2] == Bitstring("110"));
  REQUIRE(code[3] == Bitstring("111"));
}

TEST_CASE("Canonical Huffman codes, medium example", "[Huffman]") {
  // Jump straight to big codes, with no tiny code sizes in between. This
  // would be typical of cases where the code frequencies are uniformly
  // distributed.
  vector<int> bit_count;
  bit_count.push_back(4);
  bit_count.push_back(4);
  bit_count.push_back(4);
  bit_count.push_back(5);
  const vector<Bitstring> code = CanonicalHuffmanCode(bit_count);
  REQUIRE(code[0] == Bitstring("0000"));
  REQUIRE(code[1] == Bitstring("0001"));
  REQUIRE(code[2] == Bitstring("0010"));
  REQUIRE(code[3] == Bitstring("00110"));
}

TEST_CASE("Package with zero coins. Empty package.", "[Huffman]") {
  Package p;
  REQUIRE(p.value == 0);
  REQUIRE(p.bit_count.size() == 0);
}

TEST_CASE("Package with just one coin. ie: one coin", "[Huffman]") {
  Package p(1234, 7);
  REQUIRE(p.value == 1234);
  REQUIRE(p.bit_count[0] == 0);
  REQUIRE(p.bit_count[6] == 0);
  REQUIRE(p.bit_count[7] == 1);
}

TEST_CASE("Package copy constructor", "[Huffman]") {
  Package p(1234, 7);
  Package c(p);
  REQUIRE(c.value == 1234);
  REQUIRE(c.bit_count[0] == 0);
  REQUIRE(c.bit_count[6] == 0);
  REQUIRE(c.bit_count[7] == 1);
}

TEST_CASE("Merge two packages", "[Huffman]") {
  Package a(111, 3);
  Package b(222, 3);
  Package p(a, b);
  REQUIRE(p.value == 333);
  REQUIRE(p.bit_count[0] == 0);
  REQUIRE(p.bit_count[2] == 0);
  REQUIRE(p.bit_count[3] == 2);
}

TEST_CASE("Merge four packages", "[Huffman]") {
  Package p(
    Package(
      Package(1, 2),
      Package(1, 3)
    ),
    Package(
      Package(1, 3),
      Package(1, 4)
    )
  );
  REQUIRE(p.value == 4);
  REQUIRE(p.bit_count[0] == 0);
  REQUIRE(p.bit_count[1] == 0);
  REQUIRE(p.bit_count[2] == 1);
  REQUIRE(p.bit_count[3] == 2);
  REQUIRE(p.bit_count[4] == 1);
}

TEST_CASE("Merge 42 packages", "[Huffman]") {
  Package p(1, 7);
  for (int i = 0; i < 41; ++i) {
    uint64 value = i;
    int symbol = i % 9;
    Package other(value, symbol);
    p = Package(p, other);
  }
  REQUIRE(p.value == 821);
  REQUIRE(p.bit_count[0] == 5);
  REQUIRE(p.bit_count[1] == 5);
  REQUIRE(p.bit_count[2] == 5);
  REQUIRE(p.bit_count[3] == 5);
  REQUIRE(p.bit_count[4] == 5);
  REQUIRE(p.bit_count[5] == 4);
  REQUIRE(p.bit_count[6] == 4);
  REQUIRE(p.bit_count[7] == 5);
  REQUIRE(p.bit_count[8] == 4);
}

TEST_CASE("Merge list of packages", "[Huffman]") {
  vector<Package> v;
  v.push_back(Package(7, 3));
  for (int i = 0; i < 41; ++i) {
    uint64 value = i;
    int symbol = i % 9;
    v.push_back(Package(value, symbol));
  }
  Package p(v);
  REQUIRE(p.value == 827);
  REQUIRE(p.bit_count[0] == 5);
  REQUIRE(p.bit_count[1] == 5);
  REQUIRE(p.bit_count[2] == 5);
  REQUIRE(p.bit_count[3] == 6);
  REQUIRE(p.bit_count[4] == 5);
  REQUIRE(p.bit_count[5] == 4);
  REQUIRE(p.bit_count[6] == 4);
  REQUIRE(p.bit_count[7] == 4);
  REQUIRE(p.bit_count[8] == 4);
}

TEST_CASE("Package comparison by value", "[Huffman]") {
  Package a(111, 7);
  Package b(222, 3);
  REQUIRE(a < b);
  REQUIRE(!(b < a));
  REQUIRE(!(a < a));
}
