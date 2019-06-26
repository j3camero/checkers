#ifndef _ENUMERATOR_H_
#define _ENUMERATOR_H_

#include "board.h"
#include "combinator.h"
#include "pawn-cache.h"
#include "six-tuple.h"
#include "std.h"
#include "types.h"

// Enumerates a set of related checkerboard positions.
class Enumerator {
 public:
  Enumerator(int nbk, int nwk, int nbp, int nwp, int rbp, int rwp);
  Enumerator(SixTuple db);
  ~Enumerator();

  // Returns the total number of positions enumerated by this enumerator.
  uint64 NumPositions() const;

  // Select the next arrangement of checkers.
  bool Increment();

  // Cycles through several arrangements of the checkers. Deindex is faster.
  bool Increment(uint64 count);

  // Resets the enumerator to the intial position 0.
  //
  // The enumerator can cycle multiple times without needing to call this
  // function. It's only needed if you want to interrupt the enumerator partway
  // through an iteration in order to reset it to zero.
  void Reset();

  // Return the current index.
  uint64 Index() const;

  // Sets the state to correspond to the index new_index.
  void Deindex(uint64 new_index);

  // Gets a reference to the Board. It's immutable but can be copied if needed.
  const Board& GetBoard() const;

  // Deep equality operators. Checks that the positions, combinators, and other
  // stored state match.
  bool operator==(const Enumerator& other) const;
  bool operator!=(const Enumerator& other) const;

  // Shallow equality operators. Only checks that the boards match. Mostly
  // intended for unit testing convenience.
  bool operator==(const Board& b) const;
  bool operator!=(const Board& b) const;

  // Support stream-style printing of the enumerator using the << operator.
  friend ostream& operator<<(ostream &out, const Enumerator& e);

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

  // Contains [nbp, nwp, nbk, nwk, rbp, rwp]. Represents the name of the current
  // database slice. ex: 3212.43
  SixTuple db;
  Board board;  // The checkerboard.
  // The current index. Even though this could be re-calculated using the
  // combinators, it is stored here for speed.
  uint64 index;
  Combinator bp;  // Cycles through the diferent arrangements of black pawns.
  Combinator* wp;  // Pointer because white pawns depend on black pawns.
  Combinator bk;  // Cycles through the diferent arrangements of black kings.
  Combinator wk;  // Cycles through the diferent arrangements of white kings.
  // Which square numbers are currently reserved for each piece type.
  vector<int> bp_squares;
  vector<int> wp_squares;
  vector<int> bk_squares;
  vector<int> wk_squares;
  // Stores the number of arrangements of white pawns for each arrangement of
  // black pawns. Used for indexing and deindexing.
  const PawnCache& pc;
  uint64 num_positions;  // The total number of positions enumerated.
};

#endif
