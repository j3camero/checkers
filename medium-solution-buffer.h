#ifndef _MEDIUM_SOLUTION_BUFFER_H_
#define _MEDIUM_SOLUTION_BUFFER_H_

#include "small-solution-buffer.h"
#include "solution.h"
#include "std.h"

// Stores solution values for a block of related game positions.
class MediumSolutionBuffer {
 public:
  MediumSolutionBuffer(uint64 length);
  MediumSolutionBuffer(const string& filename);
  Solution Get(uint64 index) const;
  void Set(uint64 index, Solution value);
  uint64 Length();
  bool Write(const string& filename) const;

 private:
  bool Read(const string& filename);

  uint64 length;
  vector<SmallSolutionBuffer> data;
};

#endif
