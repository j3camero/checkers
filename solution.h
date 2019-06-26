#ifndef _SOLUTION_H_
#define _SOLUTION_H_

#include "std.h"

// Game outcome value.
enum Solution {
  Unknown = 0,
  Win = 1,
  Loss = 2,
  Draw = 3
};

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
  vector<MediumSolutionBuffer> data;
};

// A shorter alias for code readability.
typedef LargeSolutionBuffer SolutionBuffer;

#endif
