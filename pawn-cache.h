#ifndef _PAWN_CACHE_H_
#define _PAWN_CACHE_H_

#include "six-tuple.h"
#include "std.h"
#include "types.h"

// Stores the number of arrangements of white pawns, for each arrangement of
// black pawns. These numbers are cached to ensure that they are not needlessly
// recomputed. The cached data consist of lists of 64-bit integers that are
// used frequently for indexing and deindexing checkers positions. To query
// the cache:
//
//   const PawnCache& p = PawnCache::Get(SixTuple(3, 2, 1, 2, 4, 3));
//
// The various fields such as MaxBP, MaxWP, and NumWP(i) are defined in
// Lake (1994) and also described briefly here.
class PawnCache {
 public:
  // Initializes a new pawn cache record. To avoid repeating the calculation
  // needlessly, use PawnCache::Get() instead. This constructor is for internal
  // use only.
  PawnCache(const SixTuple& db);

  // Gets a record from the pawn cache. If none exists, it is created.
  static const PawnCache& Get(const SixTuple& db);

  // The number of black pawn arragements. ie: the "length" of this record.
  uint64 MaxBP() const;

  // Defined in Lake (1994). Returns the number of white pawn arrangements for
  // each different arrangement of black pawns for 0 <= i < MaxBP.
  uint64 NumWP(int i) const;

  // The sum of NumWP(k) for 0 <= k < i. Note that SumWP(i) does not include
  // NumWP(i). This is here for convenience and is not defined in Lake (1994).
  uint64 SumWP(int i) const;

  // The total number of white pawn arrangements, summed over all black pawn
  // arrangements. The sum of all NumWP(i) for 0 <= i < MaxBP.
  uint64 MaxWP() const;

  // Decomposes a combined pawn index into two sub-indices for black and white
  // pawns.
  //   index - (input) a combined pawn index. 0 <= index < MaxWP.
  //   bp_index - (output) pointer to where to store the black pawn index.
  //   wp_index - (output) pointer to where to store the white pawn index.
  //
  // Returns true on success, or false in case of an error.
  bool DecomposeIndex(
    const uint64 index, uint64* bp_index, uint64* wp_index) const;

 private:
  vector<uint64> num_wp;
  vector<uint64> sum_wp;
  uint64 max_wp;
};

#endif
