#include "bit-string.h"
#include "huffman.h"
#include "std.h"

vector<Bitstring> CanonicalHuffmanCode(const vector<int>& bit_count) {
  vector<Bitstring> canon;
  Bitstring code("0");
  for (int i = 0; i < bit_count.size(); ++i) {
    while (code.Size() < bit_count[i]) {
      //code.Append(false);
      code = Bitstring("0") + code;
    }
    canon.push_back(code);
    code.Increment();
  }
  return canon;
}

vector<Bitstring> LengthLimitedHuffmanCode(
  const vector<uint64>& freq,
  int max_length) {
  vector<Package> coins;
  return vector<Bitstring>();
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
