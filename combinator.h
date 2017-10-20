#ifndef _COMBINATOR_H_
#define _COMBINATOR_H_

#include <vector>

#include "board.h"
#include "types.h"

// Incrementally enumerate all C(n,k) combinations of k items from a set of n.
class Combinator {
 public:
  Combinator(int n, int k);
  int GetN();
  int GetK();
  int GetCounter(int i);
  bool Increment();
  bool Increment(uint64 count);
  bool Increment(const std::vector<int>* permutation_vector,
                 Board* board_to_update);
  bool Increment(uint64 count,
                 const std::vector<int>* permutation_vector,
                 Board* board_to_update);
  uint64 Index();
  void Deindex(uint64 new_index);

 private:
  int n;
  int k;
  uint64 index;
  std::vector<int> counters;
};

#endif
