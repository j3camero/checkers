#ifndef _SMALL_SOLUTION_BUFFER_H_
#define _SMALL_SOLUTION_BUFFER_H_

#include "solution.h"
#include "std.h"

// Stores 32 solution values using a 64-bit integer (2 bits per value).
class SmallSolutionBuffer {
 public:
  // Initializes all 32 values to Unknown.
  SmallSolutionBuffer();

  // Copy constructor. Copies the 32 values.
  SmallSolutionBuffer(const SmallSolutionBuffer& other);

  // Get and set the 32 values.
  Solution Get(int index) const;
  void Set(int index, Solution value);

 private:
  // The solution values are stored in this 64-bit integer. 32 values are stored
  // using 2 bits each.
  uint64 data;
};

#endif
