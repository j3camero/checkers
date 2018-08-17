#ifndef _SOLUTION_BUFFER_H_
#define _SOLUTION_BUFFER_H_

#include <string>
#include <vector>

#include "types.h"

// Game outcome value.
enum Solution {
  Unknown = 0,
  Win = 1,
  Loss = 2,
  Draw = 3
};

// Stores solution values for a block of related game positions.
class MediumSolutionBuffer {
 public:
  MediumSolutionBuffer(uint64 length);
  MediumSolutionBuffer(const std::string& filename);
  Solution Get(uint64 index);
  void Set(uint64 index, Solution value);
  uint64 Length();
  bool Write(const std::string& filename);

 private:
  bool Read(const std::string& filename);

  uint64 length;
  std::vector<uint64> data;
};

#endif
