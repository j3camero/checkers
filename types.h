#ifndef _TYPES_H_
#define _TYPES_H_

#include <stdint.h>

typedef uint64_t uint64;

// Game solution values.
enum Solution {
  Unknown = 0,
  Win = 1,
  Loss = 2,
  Draw = 3
};

#endif
