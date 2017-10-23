#include "enumerator.h"

#include <vector>

#include "board.h"
#include "combinator.h"
#include "types.h"

Enumerator::Enumerator(int nbk, int nwk, int nbp, int nwp, int rbp, int rwp)
  // Big-ass initializer list.
  : nbk(nbk), nwk(nwk), nbp(nbp), nwp(nwp), rbp(rbp), rwp(rwp), index(0),
    bp(4 * (rbp + 1), nbp), bk(32 - nbp - nwp, nbk),
    wk(32 - nbp - nwp - nbk, nwk) {
  // Here's the actual start of the function.
  max_bp = Choose(4 * (rbp + 1), nbp);
  if (rbp > 0) {
    max_bp -= Choose(4 * rbp, nbp);
  }
  max_bk = bk.NumCombinations();
  max_wk = wk.NumCombinations();
  // List the square numbers available to black pawns. They are in reverse
  // order to make the iteration indexes contiguous. All other pieces types
  // are in forward order regardless of color. This is one detail of
  // Lake & Shaeffer's 1994 paper that I couldn't sort out so I came up with
  // this system instead. It will result in all the same endgame database
  // "slices" with the same numbers of positions each - potentially ordered
  // differently. The end result will be the same.
  for (int i = 4 * rbp + 3; i >= 0; --i) {
    bp_squares.push_back(i);
  }
  // Place nbp black checkers on the board.
  for (int i = 0; i < nbp; ++i) {
    board.SetPiece(bp_squares[i], BlackPawn);
  }
  // Do a "warmup lap". Cycle through all arrangements of black pawns. For
  // each, calculate the number of possible arrangements of white pawns.
  max_wp = 0;
  for (uint64 i = 0; i < max_bp; ++i) {
    const int min_square = 4 * (7 - rwp);
    int avail_rank = 0;
    for (int j = 0; j < 4; ++j) {
      if (board.GetPiece(min_square + j) == Empty) {
        ++avail_rank;
      }
    }
    int avail_rest = 0;
    for (int j = min_square + 4; j < 32; ++j) {
      if (board.GetPiece(j) == Empty) {
        ++avail_rest;
      }
    }
    num_wp.push_back(Choose(avail_rank + avail_rest, nwp));
    if (avail_rest > 0) {
      num_wp[i] -= Choose(avail_rest, nwp);
    }
    max_wp += num_wp[i];
    sum_wp.push_back(max_wp);
    bp.Increment(&bp_squares, &board);
  }
  // max_bp is not used to calcualte the total because max_wp is already the
  // total summed over every arrangement of black pawns.
  num_positions = max_wp * max_bk * max_wk;
  bp.Reset();
  wp = NULL;
}

Enumerator::~Enumerator() {
  if (wp) {
    delete wp;
  }
}

uint64 Enumerator::NumPositions() const {
  return num_positions;
}

bool Enumerator::Increment() {
  // Make proper increment.
  ++index;
  return true;
}

bool Enumerator::Increment(uint64 count) {
  bool any_true = false;
  while (count > 0) {
    any_true = any_true || Increment();
    --count;
  }
  return any_true;
}

uint64 Enumerator::Index() const {
  return index;
}

void Enumerator::Deindex(uint64 new_index) {
  // Make deindex.
}

int Enumerator::NumBlackKings() const {
  return nbk;
}

int Enumerator::NumWhiteKings() const {
  return nwk;
}

int Enumerator::NumBlackPawns() const {
  return nbp;
}

int Enumerator::NumWhitePawns() const {
  return nwp;
}

int Enumerator::LeadingBlackPawnRank() const {
  return rbp;
}

int Enumerator::LeadingWhitePawnRank() const {
  return rwp;
}

bool Enumerator::operator==(const Enumerator& other) const {
  if (nbk != other.nbk || nwk != other.nwk || nbp != other.nbp ||
      nwp != other.nwp || rbp != other.rbp || rwp != other.rwp) {
    return false;
  }
  if (board != other.board) {
    return false;
  }
  if (!wp || !other.wp) {
    return false;
  }
  if (bp != other.bp || *wp != *other.wp || bk != other.bk || wk != other.wk) {
    return false;
  }
  return true;
}

bool Enumerator::operator!=(const Enumerator& other) const {
  return !(*this == other);
}

bool Enumerator::operator==(const Board& b) const {
  return board == b;
}

bool Enumerator::operator!=(const Board& b) const {
  return !(*this == b);
}

std::ostream& operator<<(std::ostream &out, const Enumerator& e) {
  out << "Enumerator(" << e.nbk << e.nwk << e.nbp << e.nwp << "."
      << e.rbp << e.rwp << ") index: " << e.Index() << std::endl
      << "bp: " << e.bp << std::endl << "wp: " << (*e.wp) << std::endl
      << "bk: " << e.bk << std::endl << "wk: " << e.wk << std::endl << e.board;
  return out;
}
