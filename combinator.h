#ifndef _COMBINATOR_H_
#define _COMBINATOR_H_

#include <vector>

#include "board.h"
#include "types.h"

uint64 Factorial(unsigned int n);
uint64 Choose(unsigned int n, unsigned int k);

// Incrementally enumerate all C(n,k) combinations of k items from a set of n.
class Combinator {
 public:
  Combinator(int n, int k);
  int GetN() const;
  int GetK() const;

  // Get the index of the i-th currently selected item. 0<=i<k and returns [0,n)
  int GetCounter(int i) const;

  // Increment by one. Select the next combination.
  bool Increment();

  // Increments several times. Deindex() is more effecient. Don't use this.
  bool Increment(uint64 count);

  // Increment by one, and move pieces accordingly on a checkerboard.
  // permutation_vector contains all the square numbers that the combinator is
  // iterating through, so the updates can be applied to board_to_update.
  bool Increment(const std::vector<int>* permutation_vector,
                 Board* board_to_update);

  // Increments several times. Deindex() is more effecient. Don't use this.
  bool Increment(uint64 count,
                 const std::vector<int>* permutation_vector,
                 Board* board_to_update);

  // Returns Choose(n, k). Useful when you have the combinator object handy,
  // but not n and k.
  uint64 NumCombinations() const;

  // Return the index of the currently selected k-combination.
  uint64 Index() const;

  // Sets the counters to correspond to the index new_index.
  void Deindex(const uint64 new_index);

  // Sets the counters to correspond to the index new_index. Also places
  // pieces of the given piece type on a checkerboard.
  void Deindex(const uint64 new_index,
               Piece piece,
               const std::vector<int>* permutation_vector,
               Board* board_to_update);

  // Resets the Combinator to its initial position, at index zero.
  void Reset();

  // Equality operators. Checks that the same k-combination is selected, not
  // just that n and k match.
  bool operator==(const Combinator& other) const;
  bool operator!=(const Combinator& other) const;

  // Support stream-style printing of the combinator using the << operator.
  friend std::ostream& operator<<(std::ostream &out, const Combinator& c);

 private:
  int n;
  int k;
  uint64 index;
  std::vector<int> counters;
};

#endif
