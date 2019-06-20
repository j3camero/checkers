#ifndef _LARGE_SOLUTION_BUFFER_H_
#define _LARGE_SOLUTION_BUFFER_H_

#include <string>
#include <vector>

#include "medium-solution-buffer.h"
#include "types.h"

// Stores solution values for a block of related game positions.
//
// Specifically designed to accomodate enormous buffer sizes in memory.
// Organized internally as a "list of lists" of small buffers to make
// allocating memory easier for the operating system.
class LargeSolutionBuffer {
 public:
  LargeSolutionBuffer(uint64 length);
  Solution Get(uint64 index);
  void Set(uint64 index, Solution value);
  uint64 Length();

 private:
  uint64 length;
  std::vector<MediumSolutionBuffer> data;
};

#endif
