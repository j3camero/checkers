#ifndef _MEDIUM_SOLUTION_BUFFER_H_
#define _MEDIUM_SOLUTION_BUFFER_H_

#include <string>
#include <vector>

#include "small-solution-buffer.h"
#include "types.h"

// Stores solution values for a block of related game positions.
class MediumSolutionBuffer {
 public:
  MediumSolutionBuffer(uint64 length);
  MediumSolutionBuffer(const std::string& filename);
  Solution Get(uint64 index) const;
  void Set(uint64 index, Solution value);
  uint64 Length();
  bool Write(const std::string& filename) const;

 private:
  bool Read(const std::string& filename);

  uint64 length;
  std::vector<SmallSolutionBuffer> data;
};

#endif
