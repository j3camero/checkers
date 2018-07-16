#include "pawn-cache.h"

#include <map>
#include <vector>

#include "combinator.h"
#include "six-tuple.h"
#include "types.h"

PawnCache::PawnCache(const SixTuple& db) {
  // Calculate the available squares for black pawns.
  std::vector<int> bp_squares;
  for (int i = 4 * db.rbp + 3; i >= 0; --i) {
    bp_squares.push_back(i);
  }
  // Place black pawns in their starting configuration on a board.
  Board b;
  for (int i = 0; i < db.nbp; ++i) {
    b.SetPiece(bp_squares[i], BlackPawn);
  }
  // Calculate the number of arrangements of black pawns.
  uint64 max_bp = Choose(4 * (db.rbp + 1), db.nbp);
  if (db.rbp > 0) {
    max_bp -= Choose(4 * db.rbp, db.nbp);
  }
  // The combinator is used to iterate through all max_bp arrangements of black
  // pawns. It takes care of moving the black pawns around on the board too.
  Combinator c(4 * (db.rbp + 1), db.nbp);
  // Do a "warmup lap". Cycle through all arrangements of black pawns. For
  // each, calculate the number of possible arrangements of white pawns.
  max_wp = 0;
  for (uint64 i = 0; i < max_bp; ++i) {
    const int min_square = 4 * (7 - db.rwp);
    int avail_rank = 0;
    for (int j = 0; j < 4; ++j) {
      if (b.GetPiece(min_square + j) == Empty) {
        ++avail_rank;
      }
    }
    int avail_rest = 0;
    for (int j = min_square + 4; j < 32; ++j) {
      if (b.GetPiece(j) == Empty) {
        ++avail_rest;
      }
    }
    num_wp.push_back(Choose(avail_rank + avail_rest, db.nwp));
    if (avail_rest > 0) {
      num_wp[i] -= Choose(avail_rest, db.nwp);
    }
    sum_wp.push_back(max_wp);
    max_wp += num_wp[i];
    c.Increment(&bp_squares, &b);
  }
}

// This comparison operator imposes a strict ordering on the six-tuples, as
// required by std::map.
struct SixTupleCompare {
   bool operator() (const SixTuple& a, const SixTuple& b) const {
       if (a.nbp < b.nbp) return true;
       if (a.nbp > b.nbp) return false;
       if (a.nwp < b.nwp) return true;
       if (a.nwp > b.nwp) return false;
       if (a.nbk < b.nbk) return true;
       if (a.nbk > b.nbk) return false;
       if (a.nwk < b.nwk) return true;
       if (a.nwk > b.nwk) return false;
       if (a.rbp < b.rbp) return true;
       if (a.rbp > b.rbp) return false;
       if (a.rwp < b.rwp) return true;
       if (a.rwp > b.rwp) return false;
       return false;
   }
};

// This is the global in-memory pawn cache.
std::map<SixTuple, PawnCache, SixTupleCompare> cache;

const PawnCache& PawnCache::Get(const SixTuple& db) {
  std::map<SixTuple, PawnCache, SixTupleCompare>::iterator it = cache.find(db);
  if (it == cache.end()) {
    // No entry was found in the cache. Create it. This is the expensive
    // calculation that that pawn cache is designed to avoid.
    cache.insert(std::pair<SixTuple, PawnCache>(db, PawnCache(db)));
  }
  return cache.find(db)->second;
}

uint64 PawnCache::MaxBP() const {
  return num_wp.size();
}

uint64 PawnCache::NumWP(int i) const {
  return num_wp[i];
}

uint64 PawnCache::SumWP(int i) const {
  return sum_wp[i];
}

uint64 PawnCache::MaxWP() const {
  return max_wp;
}

bool PawnCache::DecomposeIndex(const uint64 index,
                               uint64* bp_index,
                               uint64* wp_index) const {
  if (index >= max_wp || !bp_index || !wp_index) {
    return false;
  }
  uint64 lo = 0;
  uint64 hi = sum_wp.size();
  while (hi - lo > 1) {
    uint64 mid = (hi + lo) / 2;
    if (index < sum_wp[mid]) {
      hi = mid;
    } else {
      lo = mid;
    }
  }
  *bp_index = lo;
  *wp_index = index - sum_wp[lo];
  return true;
}
