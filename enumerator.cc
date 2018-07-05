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
  wp = NULL;
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
  SetupBlackPawns();
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
    sum_wp.push_back(max_wp);
    max_wp += num_wp[i];
    bp.Increment(&bp_squares, &board);
  }
  // max_bp is not used to calcualte the total because max_wp is already the
  // total summed over every arrangement of black pawns.
  num_positions = max_wp * max_bk * max_wk;
  // Reset the pieces to their starting positions.
  Reset();
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
  ++index;
  if (!wk.Increment(&wk_squares, &board)) {
    return false;
  }
  board.Clear(WhiteKing);
  if (!bk.Increment(&bk_squares, &board)) {
    SetupWhiteKings();
    return false;
  }
  board.Clear(BlackKing);
  if (!wp->Increment(&wp_squares, &board) && wp->Index() < num_wp[bp.Index()]) {
    SetupBlackKings();
    SetupWhiteKings();
    return false;
  }
  board.Clear(WhitePawn);
  if (!bp.Increment(&bp_squares, &board)) {
    // The if statements are separated this way so that it's clear the
    // Increment() happens before the Index().
    if (bp.Index() < max_bp) {
      SetupWhitePawns();
      SetupBlackKings();
      SetupWhiteKings();
      return false;
    }
  }
  Reset();
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

void Enumerator::Reset() {
  board.Clear();
  bp.Reset();
  SetupBlackPawns();
  SetupWhitePawns();
  SetupBlackKings();
  SetupWhiteKings();
  index = 0;
}

uint64 Enumerator::Index() const {
  return index;
}

void Enumerator::Deindex(uint64 new_index) {
  if (new_index >= num_positions) {
    throw "Index is too large.";
  }
  board.Clear();
  index = new_index;
  // Use division & modulus to find the indices for the kings.
  const uint64 wkr = new_index / max_wk;
  const uint64 bkr = wkr / max_bk;
  const uint64 wk_index = new_index - max_wk * wkr;
  const uint64 bk_index = wkr - max_bk * bkr;
  // Quickly find out the pawn indices by binary searching sum_wp. This
  // can't be done using division & modulus because the white pawn cycles
  // have differing lengths.
  uint64 lo = 0;
  uint64 hi = sum_wp.size();
  while (hi - lo > 1) {
    uint64 mid = (hi + lo) / 2;
    if (bkr < sum_wp[mid]) {
      hi = mid;
    } else {
      lo = mid;
    }
  }
  const uint64 bp_index = lo;
  const uint64 wp_index = bkr - sum_wp[bp_index];
  // Black pawns.
  bp.Deindex(bp_index, BlackPawn, &bp_squares, &board);
  // White pawns.
  if (wp) {
    delete wp;
  }
  wp_squares.clear();
  for (int i = 4 * (7 - rwp); i < 32; ++i) {
    if (board.GetPiece(i) == Empty) {
      wp_squares.push_back(i);
    }
  }
  wp = new Combinator(wp_squares.size(), nwp);
  wp->Deindex(wp_index, WhitePawn, &wp_squares, &board);
  // Black kings.
  bk_squares.clear();
  for (int i = 0; i < 32; ++i) {
    if (board.GetPiece(i) == Empty) {
      bk_squares.push_back(i);
    }
  }
  bk.Deindex(bk_index, BlackKing, &bk_squares, &board);
  // White kings.
  wk_squares.clear();
  for (int i = 0; i < 32; ++i) {
    if (board.GetPiece(i) == Empty) {
      wk_squares.push_back(i);
    }
  }
  wk.Deindex(wk_index, WhiteKing, &wk_squares, &board);
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

void Enumerator::SetupBlackPawns() {
  for (int i = 0; i < nbp; ++i) {
    board.SetPiece(bp_squares[i], BlackPawn);
  }
}

void Enumerator::SetupWhitePawns() {
  if (wp) {
    delete wp;
  }
  wp_squares.clear();
  for (int i = 4 * (7 - rwp); i < 32; ++i) {
    if (board.GetPiece(i) == Empty) {
      wp_squares.push_back(i);
    }
  }
  wp = new Combinator(wp_squares.size(), nwp);
  for (int i = 0; i < nwp; ++i) {
    board.SetPiece(wp_squares[i], WhitePawn);
  }
}

void Enumerator::SetupBlackKings() {
  bk.Reset();
  bk_squares.clear();
  for (int i = 0; i < 32; ++i) {
    if (board.GetPiece(i) == Empty) {
      bk_squares.push_back(i);
    }
  }
  for (int i = 0; i < nbk; ++i) {
    board.SetPiece(bk_squares[i], BlackKing);
  }
}

void Enumerator::SetupWhiteKings() {
  wk.Reset();
  wk_squares.clear();
  for (int i = 0; i < 32; ++i) {
    if (board.GetPiece(i) == Empty) {
      wk_squares.push_back(i);
    }
  }
  for (int i = 0; i < nwk; ++i) {
    board.SetPiece(wk_squares[i], WhiteKing);
  }
}
