#include "combinator.h"

#include <vector>

#include "board.h"
#include "types.h"

Combinator::Combinator(int n, int k): n(n), k(k) {
  if (k > n) {
    throw "n has to be larger than k.";
  }
  index = 0;
  for (int i = 0; i < k; ++i) {
    counters.push_back(i);
  }
}

int Combinator::GetN() {
  return n;
}

int Combinator::GetK() {
  return k;
}

int Combinator::GetCounter(int i) {
  return counters[i];
}

bool Combinator::Increment() {
  return Increment(NULL, NULL);
}

bool Combinator::Increment(const std::vector<int>* permutation_vector,
                           Board* board_to_update) {
  ++index;
  int i = k - 1;
  for (int t = n - 1; i >= 0 && counters[i] == t; --t) {
    --i;
  }
  int r = 0;
  if (i >= 0) {
    if (board_to_update) {
      const int from = permutation_vector->at(counters[i]);
      const int to = permutation_vector->at(counters[i] + 1);
      board_to_update->MovePiece(from, to);
    }
    counters[i]++;
    r = counters[i] + 1;
  } else {
    index = 0;
  }
  for (int j = i + 1; j < k; ++j) {
    if (board_to_update) {
      const int from = permutation_vector->at(counters[j]);
      const int to = permutation_vector->at(r);
      board_to_update->MovePiece(from, to);
    }
    counters[j] = r;
    ++r;
  }
  return i < 0;
}

bool Combinator::Increment(uint64 count) {
  bool any_true = false;
  while (count > 0) {
    any_true = any_true || Increment();
    --count;
  }
  return any_true;
}

bool Combinator::Increment(uint64 count,
                           const std::vector<int>* permutation_vector,
                           Board* board_to_update) {
  bool any_true = false;
  while (count > 0) {
    any_true = any_true || Increment(permutation_vector, board_to_update);
    --count;
  }
  return any_true;
}

uint64 Combinator::Index() {
  return index;
}

void Combinator::Deindex(uint64 new_index) {
  index = new_index;
}
