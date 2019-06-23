#ifndef _HUFFMAN_H_
#define _HUFFMAN_H_

#include <vector>

#include "bit-string.h"

std::vector<Bitstring> CanonicalHuffmanCode(const std::vector<int>& bit_count);

#endif
