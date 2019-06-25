#include "huffman.h"
#include "catch.hpp"

TEST_CASE("Canonical Huffman codes, minimal cases", "[Huffman]") {
  std::vector<int> bit_count;
  const std::vector<Bitstring> zero = CanonicalHuffmanCode(bit_count);
  REQUIRE(zero.size() == 0);
  bit_count.push_back(1);
  const std::vector<Bitstring> one = CanonicalHuffmanCode(bit_count);
  REQUIRE(one.size() == 1);
  REQUIRE(one[0] == Bitstring("0"));
}

TEST_CASE("Canonical Huffman codes, small example", "[Huffman]") {
  std::vector<int> bit_count;
  bit_count.push_back(1);
  bit_count.push_back(2);
  bit_count.push_back(3);
  bit_count.push_back(3);
  const std::vector<Bitstring> code = CanonicalHuffmanCode(bit_count);
  REQUIRE(code[0] == Bitstring("0"));
  REQUIRE(code[1] == Bitstring("10"));
  REQUIRE(code[2] == Bitstring("110"));
  REQUIRE(code[3] == Bitstring("111"));
}

TEST_CASE("Canonical Huffman codes, medium example", "[Huffman]") {
  // Jump straight to big codes, with no tiny code sizes in between. This
  // would be typical of cases where the code frequencies are uniformly
  // distributed.
  std::vector<int> bit_count;
  bit_count.push_back(4);
  bit_count.push_back(4);
  bit_count.push_back(4);
  bit_count.push_back(5);
  const std::vector<Bitstring> code = CanonicalHuffmanCode(bit_count);
  REQUIRE(code[0] == Bitstring("0000"));
  REQUIRE(code[1] == Bitstring("0001"));
  REQUIRE(code[2] == Bitstring("0010"));
  REQUIRE(code[3] == Bitstring("00110"));
}

TEST_CASE("Package with just one coin. ie: one coin", "[Huffman]") {
  Package p(1234, 7);
  REQUIRE(p.value == 1234);
  REQUIRE(p.bit_count[0] == 0);
  REQUIRE(p.bit_count[6] == 0);
  REQUIRE(p.bit_count[7] == 1);
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
