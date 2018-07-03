#ifndef _ENUMERATOR_H_
#define _ENUMERATOR_H_

#include <vector>

#include "board.h"
#include "combinator.h"
#include "types.h"

// Enumerates a set of related checkerboard positions.
class Enumerator {
 public:
  Enumerator(int nbk, int nwk, int nbp, int nwp, int rbp, int rwp);
  ~Enumerator();

  // Returns the total number of positions enumerated by this enumerator.
  uint64 NumPositions() const;

  // Select the next arrangement of checkers.
  bool Increment();

  // Cycles through several arrangements of the checkers. Deindex is faster.
  bool Increment(uint64 count);

  // Return the current index.
  uint64 Index() const;

  // Sets the state to correspond to the index new_index.
  void Deindex(uint64 new_index);

  // Getters for the 6 parameters that identify this enumerator.
  int NumBlackKings() const;
  int NumWhiteKings() const;
  int NumBlackPawns() const;
  int NumWhitePawns() const;
  int LeadingBlackPawnRank() const;
  int LeadingWhitePawnRank() const;

  // Deep equality operators. Checks that the positions, combinators, and other
  // stored state match.
  bool operator==(const Enumerator& other) const;
  bool operator!=(const Enumerator& other) const;

  // Shallow equality operators. Only checks that the boards match. Mostly
  // intended for unit testing convenience.
  bool operator==(const Board& b) const;
  bool operator!=(const Board& b) const;

  // Support stream-style printing of the enumerator using the << operator.
  friend std::ostream& operator<<(std::ostream &out, const Enumerator& e);

 private:
   // Set up the black pawns on the board. Assumes that the board stars empty.
   void SetupBlackPawns();

   // Set up a white pawn cycle. Assumes black pawns are already placed, and
   // there are no other piece types in the way.
   // Makes a list of the squares that are not already taken by other pieces,
   // initializes the white pawn combinator, and places the white pawns on the
   // board in their initial configuration, ready to iterate.
   void SetupWhitePawns();

   // Sets up a black king cycle. Assumes black pawns and white pawns are
   // already placed, but not any kings.
   void SetupBlackKings();

   // Sets up a white king cycle. Assumes all pawns and black kings are
   // already placed.
   void SetupWhiteKings();

   int nbk;  // Number of black kings.
   int nwk;  // Number of white kings.
   int nbp;  // Number of black pawns.
   int nwp;  // Number of white pawns.
   int rbp;  // Rank of the leading black pawn (0-6).
   int rwp;  // Rank of the leading white pawn (0-6).
   uint64 num_positions;  // The total number of positions enumerated.
   uint64 max_bp;  // Number of arrangements of black pawns.
   // Number of arrangements of white pawns, summed over every possible
   // arrangement of black pawns.
   uint64 max_wp;
   uint64 max_bk;  // Number of arrangements of black kings.
   uint64 max_wk;  // Number of arrangements of white kings.
   // Number of arrangements of white pawns. One for each different arrangement
   // of black pawns.
   std::vector<uint64> num_wp;
   // Cumulative sum of num_wp. Useful for faster deindexing.
   std::vector<uint64> sum_wp;
   Board board;  // The checkerboard.
   // The current index. Even though this could be re-calculated using the
   // combinators, it is stored here for speed.
   uint64 index;
   Combinator bp;  // Cycles through the diferent arrangements of black pawns.
   Combinator* wp;  // Pointer because white pawns depend on black pawns.
   Combinator bk;  // Cycles through the diferent arrangements of black kings.
   Combinator wk;  // Cycles through the diferent arrangements of white kings.
   // Which square numbers are currently reserved for each piece type.
   std::vector<int> bp_squares;
   std::vector<int> wp_squares;
   std::vector<int> bk_squares;
   std::vector<int> wk_squares;
};

#endif
