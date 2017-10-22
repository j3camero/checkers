// Stores solution values for a block of related game positions.
#ifndef _SOLUTION_BUFFER_H_
#define _SOLUTION_BUFFER_H_

#include <string>

#include "types.h"

class SolutionBuffer {
 public:
  SolutionBuffer(uint64 length);
  SolutionBuffer(const std::string& filename);
  ~SolutionBuffer();
  Solution Get(uint64 index);
  void Set(uint64 index, Solution value);
  uint64 Length();
  bool Write(const std::string& filename);

 private:
  bool Read(const std::string& filename);

  uint64 length;
  uint64 *data;
};

#endif
