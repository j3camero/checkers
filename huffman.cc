#include "huffman.h"

#include <iostream>
#include <vector>

#include "bit-string.h"

std::vector<Bitstring> CanonicalHuffmanCode(const std::vector<int>& bit_count) {
  std::vector<Bitstring> canon;
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
