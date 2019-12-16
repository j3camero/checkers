#include "bit-string.h"
#include "catch.hpp"
#include "huffman.h"
#include "std.h"

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
  REQUIRE(code[1] == Bitstring("01"));
  REQUIRE(code[2] == Bitstring("011"));
  REQUIRE(code[3] == Bitstring("111"));
  REQUIRE(Bitstring::IsPrefixFree(code));
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
  REQUIRE(code[1] == Bitstring("1000"));
  REQUIRE(code[2] == Bitstring("0100"));
  REQUIRE(code[3] == Bitstring("01100"));
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
  REQUIRE(p.bit_count[7] == 0);
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

TEST_CASE("Sort a list of packages", "[Huffman]") {
  // Make a list of packages in random order.
  vector<Package> p;
  p.push_back(Package(6, 7));
  p.push_back(Package(4, 3));
  p.push_back(Package(8, 9));
  p.push_back(Package(5, 6));
  p.push_back(Package(4, 4));
  p.push_back(Package(5, 5));
  p.push_back(Package(3, 1));
  p.push_back(Package(7, 8));
  p.push_back(Package(4, 2));
  // Sort the packages.
  sort(p.begin(), p.end());
  REQUIRE(p[0].value == 3);
  REQUIRE(p[1].value == 4);
  REQUIRE(p[2].value == 4);
  REQUIRE(p[3].value == 4);
  REQUIRE(p[4].value == 5);
  REQUIRE(p[5].value == 5);
  REQUIRE(p[6].value == 6);
  REQUIRE(p[7].value == 7);
  REQUIRE(p[8].value == 8);
  // Sort in reverse order.
  sort(p.rbegin(), p.rend());
  REQUIRE(p[0].value == 8);
  REQUIRE(p[8].value == 3);
}

TEST_CASE("Limited length Huffman code (edge cases)", "[Huffman]") {
  vector<uint64> freq;
  // Fails because zero symbol frequencies are given.
  REQUIRE_THROWS(LengthLimitedHuffmanCode(freq, 3));
  freq.push_back(3);
  // Fails because only one symbol frequency is given.
  REQUIRE_THROWS(LengthLimitedHuffmanCode(freq, 3));
  freq.push_back(1);
  // Fails because the max bit length of zero is invalid.
  REQUIRE_THROWS(LengthLimitedHuffmanCode(freq, 0));
}

TEST_CASE("Limited length Huffman code (not enough bits)", "[Huffman]") {
  vector<uint64> freq;
  freq.push_back(201);
  freq.push_back(95);
  freq.push_back(85);
  freq.push_back(50);
  freq.push_back(24);
  freq.push_back(11);
  freq.push_back(5);
  freq.push_back(2);
  freq.push_back(1);
  // Fails because there are too few bits to encode all the possible symbols.
  // 3 bits can encode 8 different symbols, but 9 symbols are given!
  REQUIRE_THROWS(LengthLimitedHuffmanCode(freq, 3));
  // Using 4 bits everything works fine.
  vector<Bitstring> code = LengthLimitedHuffmanCode(freq, 4);
  REQUIRE(code.size() == 9);
  REQUIRE(Bitstring::IsPrefixFree(code));
}

TEST_CASE("Limited length Huffman code (2 symbols)", "[Huffman]") {
  vector<uint64> freq;
  freq.push_back(3);
  freq.push_back(1);
  vector<Bitstring> code = LengthLimitedHuffmanCode(freq, 3);
  REQUIRE(code.size() == 2);
  REQUIRE(code[0] == Bitstring("0"));
  REQUIRE(code[1] == Bitstring("1"));
  REQUIRE(Bitstring::IsPrefixFree(code));
}

TEST_CASE("Limited length Huffman code (3 symbols)", "[Huffman]") {
  vector<uint64> freq;
  freq.push_back(3);
  freq.push_back(2);
  freq.push_back(1);
  vector<Bitstring> code = LengthLimitedHuffmanCode(freq, 3);
  REQUIRE(code.size() == 3);
  REQUIRE(code[0] == Bitstring("0"));
  REQUIRE(code[1] == Bitstring("01"));
  REQUIRE(code[2] == Bitstring("11"));
  REQUIRE(Bitstring::IsPrefixFree(code));
}

TEST_CASE("Limited length Huffman code (9 symbols)", "[Huffman]") {
  vector<uint64> freq;
  freq.push_back(201);
  freq.push_back(95);
  freq.push_back(85);
  freq.push_back(50);
  freq.push_back(24);
  freq.push_back(11);
  freq.push_back(5);
  freq.push_back(2);
  freq.push_back(1);
  vector<Bitstring> code = LengthLimitedHuffmanCode(freq, 4);
  REQUIRE(code.size() == 9);
  REQUIRE(Bitstring::IsPrefixFree(code));
}

TEST_CASE("Limited length Huffman code (medium example)", "[Huffman]") {
  // A few dozen symbols with an exponential frequency distribution.
  vector<uint64> freq;
  uint64 n = 999999999;
  while (n > 4) {
    freq.push_back(n);
    n -= 2;
    freq.push_back(n);
    n = n * 6 / 7;
    freq.push_back(n);
    n = n / 2 - 1;
  }
  // Squeeze the codes into shorter and shorter minimum bit lengths.
  for (int min_length = 11; min_length >= 7; --min_length) {
    vector<Bitstring> code = LengthLimitedHuffmanCode(freq, min_length);
    REQUIRE(code.size() == freq.size());
    REQUIRE(Bitstring::IsPrefixFree(code));
  }
}

TEST_CASE("Limited length Huffman code (big example)", "[Huffman]") {
  // Hundreds of symbols with a uniform distribution.
  vector<uint64> freq;
  for (uint64 i = 100; i > 0; --i) {
    freq.push_back(i);
  }
  vector<Bitstring> code = LengthLimitedHuffmanCode(freq, 9);
  REQUIRE(code.size() == freq.size());
  REQUIRE(Bitstring::IsPrefixFree(code));
}
