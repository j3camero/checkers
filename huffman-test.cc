#include "huffman.h"
#include "catch.hpp"

TEST_CASE("Canonical Huffman codes, minimal cases", "[Bitstring]") {
  std::vector<int> bit_count;
  const std::vector<Bitstring> zero = CanonicalHuffmanCode(bit_count);
  REQUIRE(zero.size() == 0);
  bit_count.push_back(1);
  const std::vector<Bitstring> one = CanonicalHuffmanCode(bit_count);
  REQUIRE(one.size() == 1);
  REQUIRE(one[0] == Bitstring("0"));
}

TEST_CASE("Canonical Huffman codes, small example", "[Bitstring]") {
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

TEST_CASE("Canonical Huffman codes, medium example", "[Bitstring]") {
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
