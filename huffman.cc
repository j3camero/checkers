#include "bit-string.h"
#include "huffman.h"
#include "std.h"

vector<Bitstring> CanonicalHuffmanCode(const vector<int>& bit_count) {
  vector<Bitstring> canon;
  Bitstring code("0");
  for (int i = 0; i < bit_count.size(); ++i) {
    while (code.Size() < bit_count[i]) {
      code = Bitstring("0") + code;
    }
    canon.push_back(code.Reverse());
    code.Increment();
  }
  return canon;
}

vector<Bitstring> LengthLimitedHuffmanCode(
  const vector<uint64>& freq,
  int max_length) {
  const int n = freq.size();
  if (n < 2 || max_length < 1 || freq.size() > (1 << max_length)) {
    throw "Failed to calculate Huffman code.";
  }
  // Use the package-merge algorithm to calculate how many bits to use to
  // encode each symbol.
  vector<Package> coins;
  for (int i = 0; i < max_length; ++i) {
    // Package.
    for (int j = 0; j < n; ++j) {
      coins.push_back(Package(freq[j], j));
    }
    sort(coins.begin(), coins.end());
    // Merge.
    vector<Package> merged_coins;
    for (int j = 0; j + 1 < coins.size(); j += 2) {
      const Package& a = coins[j];
      const Package& b = coins[j + 1];
      merged_coins.push_back(Package(a, b));
    }
    coins = merged_coins;
  }
  if (coins.size() > n - 1) {
    coins.resize(n - 1);
  }
  Package package_merge(coins);
  return CanonicalHuffmanCode(package_merge.bit_count);
}

Package::Package() : value(0), bit_count(0) {

}

Package::Package(uint64 value, int symbol)
  : value(value), bit_count(symbol + 1) {
  bit_count[symbol] = 1;
}

Package::Package(const Package& p) : value(p.value), bit_count(p.bit_count) {

}

Package::Package(const Package& a, const Package& b)
  : value(a.value + b.value) {
  for (int i = 0; i < a.bit_count.size() || i < b.bit_count.size(); ++i) {
    const int a_count = i < a.bit_count.size() ? a.bit_count[i] : 0;
    const int b_count = i < b.bit_count.size() ? b.bit_count[i] : 0;
    bit_count.push_back(a_count + b_count);
  }
}

Package::Package(const vector<Package>& p) : value(0) {
  for (int i = 0; i < p.size(); ++i) {
    value += p[i].value;
    const vector<int>& v = p[i].bit_count;
    if (bit_count.size() < v.size()) {
      bit_count.resize(v.size(), 0);
    }
    for (int j = 0; j < v.size(); ++j) {
      bit_count[j] += v[j];
    }
  }
}

bool Package::operator<(const Package& p) const {
  return value < p.value;
}
