#ifndef _HUFFMAN_H_
#define _HUFFMAN_H_

#include <vector>

#include "bit-string.h"
#include "types.h"

// Calculates the canonical Huffman code given the bit lengths of the symbols.
std::vector<Bitstring> CanonicalHuffmanCode(const std::vector<int>& bit_count);

// Calculates a set of Huffman codes with a maximum length.
std::vector<Bitstring> LengthLimitedHuffmanCode(
  const std::vector<uint64>& freq,
  int max_length
);

// Helper class for implementing the package-merge algorithm.
class Package {
 public:
  // Initialize a package with one coin in it.
  Package(uint64 value, int symbol);

  // Merge two packages. Sum their values and bit counts.
  Package(const Package& a, const Package& b);

  // The total numismatic value of this package of coins.
  uint64 value;

  // The number of bits to use to encode each symbol.
  std::vector<int> bit_count;
};

#endif
