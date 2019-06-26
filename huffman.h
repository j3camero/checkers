#ifndef _HUFFMAN_H_
#define _HUFFMAN_H_

#include "bit-string.h"
#include "std.h"

// Calculates the canonical Huffman code given the bit lengths of the symbols.
vector<Bitstring> CanonicalHuffmanCode(const vector<int>& bit_count);

// Calculates a set of Huffman codes with a maximum length.
vector<Bitstring> LengthLimitedHuffmanCode(
  const vector<uint64>& freq,
  int max_length
);

// Helper class for implementing the package-merge algorithm.
class Package {
 public:
  // Initialize an empty package with zero coins in it.
  Package();

  // Initialize a package with one coin in it.
  Package(uint64 value, int symbol);

  // Copy constructor.
  Package(const Package& p);

  // Merge two packages. Sum their values and bit counts.
  Package(const Package& a, const Package& b);

  // Merge a list of packages into one. Sum their values and bit counts.
  Package(const vector<Package>& p);

  // Comparison operator. Sorts by numismatic value.
  bool operator<(const Package& p) const;

  // The total numismatic value of this package of coins.
  uint64 value;

  // The number of bits to use to encode each symbol.
  vector<int> bit_count;
};

#endif
