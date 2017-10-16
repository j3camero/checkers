#ifndef _TWO_BIT_STRING_H_
#define _TWO_BIT_STRING_H_

#include <stdint.h>
#include <vector>

class TwoBitString {
 public:
  TwoBitString();

 private:
  std::vector<uint64_t> data;
};

#endif
