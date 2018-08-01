#include "combinator.h"

#include <vector>

#include "board.h"
#include "types.h"

std::vector<uint64> factorial_cache;
std::vector<std::vector<uint64> > choose_cache;

// Precompute factorials and binomial coefficients. This function should be
// run before any calls to Factorial(n) or Choose(n,k).
bool DoPrecomputations() {
  // I've heard that Factorial(21) exceeds 64-bit integer range.
  const int max_factorial = 20;
  uint64 f = 1;
  factorial_cache.push_back(1);
  for (int i = 1; i < max_factorial; ++i) {
    f *= i;
    factorial_cache.push_back(f);
  }
  // Binomial coefficients C(n,k) are small enough that they don't overflow,
  // for small n and k.
  const int max_choose = 50;
  for (int n = 0; n < max_choose; ++n) {
    choose_cache.push_back(std::vector<uint64>(n + 1, 1));
    for (int k = 1; k < n; ++k) {
      choose_cache[n][k] = choose_cache[n - 1][k - 1] + choose_cache[n - 1][k];
    }
  }
  return true;
}

// This line make the precomputations run before main().
bool done_precomputations = DoPrecomputations();

uint64 Factorial(unsigned int n) {
  if (n < factorial_cache.size()) {
    return factorial_cache[n];
  }
  throw "Factorial value overflows 64-bit integer range.";
}

uint64 Choose(unsigned int n, unsigned int k) {
  if (n < choose_cache.size()) {
    if (k > n) {
      return 0;
    }
    return choose_cache[n][k];
  }
  throw "Cannot calculate binomial coefficient Choose(n,k) for large n.";
}

Combinator::Combinator(int n, int k): n(n), k(k) {
  if (k > n) {
    throw "n must be larger than k.";
  }
  Reset();
}

int Combinator::GetN() const {
  return n;
}

int Combinator::GetK() const {
  return k;
}

int Combinator::GetCounter(int i) const {
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

uint64 Combinator::NumCombinations() const {
  return Choose(n, k);
}

uint64 Combinator::Index() const {
  return index;
}

void Combinator::Deindex(const uint64 new_index) {
  Deindex(new_index, Empty, NULL, NULL);
}

void Combinator::Deindex(const uint64 new_index,
                         Piece piece,
                         const std::vector<int>* permutation_vector,
                         Board* board_to_update) {
  if (new_index >= Choose(n, k)) {
    throw "Index is too large.";
  }
  index = new_index;
  uint64 rem = index;
  int c = 0;
  for (int i = 0; i < k; ++i) {
    while (Choose(n - c - 1, k - i - 1) <= rem) {
      rem -= Choose(n - c - 1, k - i - 1);
      ++c;
    }
    counters[i] = c;
    if (permutation_vector && board_to_update) {
      const int square_num = permutation_vector->at(c);
      board_to_update->SetPiece(square_num, piece);
    }
    ++c;
  }
}

void Combinator::Reset() {
  index = 0;
  counters.clear();
  for (int i = 0; i < k; ++i) {
    counters.push_back(i);
  }
}

bool Combinator::operator==(const Combinator& other) const {
  if (n != other.n || k != other.k || index != other.index) {
    return false;
  }
  for (int i = 0; i < k; ++i) {
    if (counters[i] != other.counters[i]) {
      return false;
    }
  }
  return true;
}

bool Combinator::operator!=(const Combinator& other) const {
  return !(*this == other);
}

// This is the Combinator's << operator for stream-style output. It's a friend.
std::ostream& operator<<(std::ostream &out, const Combinator& c) {
    out << "Combinator(" << c.GetN() << "," << c.GetK() << ") index: "
        << c.Index() << " counters: [";
    for (int i = 0; i < c.GetK(); ++i) {
      if (i > 0) {
        out << ", ";
      }
      out << c.GetCounter(i);
    }
    out << "]";
    return out;
}
