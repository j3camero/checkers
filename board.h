#ifndef _BOARD_H_
#define _BOARD_H_

#include "seven-tuple.h"
#include "six-tuple.h"
#include "std.h"

enum Piece {
  Empty = 0,
  WhiteKing = 1,
  BlackKing = 2,
  WhitePawn = 3,
  BlackPawn = 4
};

// Represents one checkers board position. Does move generation logic.
class Board {
 public:
  // Initialize an empty board.
  Board();

  // Copy constructor.
  Board(const Board& b);

  // Initialize a board from a human-readable string.
  Board(const string& s);

  // Initialialize a board by index. Calls Deindex().
  Board(const SevenTuple& index);
  Board(const SixTuple& db, uint64 index);
  Board(int nbk, int nwk, int nbp, int nwp, int rbp, int rwp, uint64 index);

  Piece GetPiece(int index) const;
  void SetPiece(int index, Piece p);

  // Moves the value from square 'from' to square 'to'. No check is made that
  // 'to' is empty. If not, then the piece would be overwritten.
  void MovePiece(int from, int to);

  // Sets all the spaces to empty.
  void Clear();

  // Removes all pices of a certain type from the board.
  void Clear(Piece p);

  // Determine which database slice this board position belongs to.
  SixTuple WhichDatabaseSlice() const;

  // Determine the index of this board position. The result is not defined if
  // the provided db does not match WhichDatabaseSlice(). Use this function
  // only if the caller is sure they know the correct SixTuple to use. If not,
  // use the overload of Index() that takes no arguments instead.
  uint64 Index(const SixTuple& db) const;

  // Determine the index of this board position, including which database slice.
  // Returns a SevenTuple containing which DB slice and which index.
  SevenTuple Index() const;

  // Deindex a position by its index and which database slice it's part of.
  // Replaces the current board state in-place.
  void Deindex(const SixTuple& db, uint64 index);

  // Returns the mirror of this board.
  //   - Black and white colors are reversed.
  //   - The board is rotated 180 degrees around.
  Board Mirror() const;

  // Returns the mirror of a piece type.
  static Piece MirrorPiece(Piece p);

  // Does the same thing as .Mirror().Index() but more efficiently by not
  // copying the board.
  uint64 MirrorIndex(const SixTuple& db) const;
  SevenTuple MirrorIndex() const;

  // True iff the piece is a white pawn or white king, false otherwise.
  static bool IsWhite(Piece p);

  // Calculates the mirror indices of pawn captures starting from 'from'.
  // Assumes that the piece on 'from' is a black pawn. A list of SevenTuples
  // is returned, since the captures may result in positions from multiple
  // database slices.
  bool PawnCaptures(int from, set<SevenTuple>* captures = NULL);

  // Calculates the mirror indices of king captures starting from 'from'.
  // Assumes that the piece on 'from' is a black king. A list of SevenTuples
  // is returned, since the captures may result in positions from multiple
  // database slices.
  bool KingCaptures(int from, set<SevenTuple>* captures = NULL);

  // Calculates the mirror indices of moves that advance the leading black pawn.
  // This function must be used instead of PawnMoves in exactly this situation.
  // Assumes that the piece on 'from' is a black pawn. Returns a list of indices
  // but only a single db SixTuple, since all the resulting moves flow into one
  // database slice.
  bool ConversionMoves(
    int from, vector<uint64>* moves = NULL, SixTuple* db = NULL);

  // Calculates the mirror indices of pawn moves that do not advance the leading
  // black pawn. Don't use this for moves that advance the leading black pawn,
  // or the result will be undefined. Assumes that the piece on 'from' is a
  // black pawn.
  bool PawnMoves(int from, vector<uint64>* moves = NULL);

  // Calculates the mirror indices of king moves. Assumes that the piece on
  // 'from' is a black king.
  bool KingMoves(int from, vector<uint64>* moves = NULL);

  // These are used for precomputing move and jump offset tables, not for
  // live move generation. Reasoning about the 2D shape of the board in
  // real-time would be too slow.
  static bool IsOnBoard(int x, int y);
  static int XYToSpaceNumber(int x, int y);
  static void SpaceNumberToXY(int space, int* x, int* y);

  // Equality operator.
  bool operator==(const Board& other) const;
  bool operator!=(const Board& other) const;

  // Format the board as an easy-to-read string.
  string HumanReadable() const;

  // Support stream-style printing of the board using the << operator.
  friend ostream& operator<<(ostream &out, const Board& board);

 private:
  // Calculates the mirror indices of moves that do not advance the leading
  // black pawn. max_direction = 2 for pawns and 4 for kings.
  bool NonConversionMoves(int from,
                          int max_direction,
                          vector<uint64>* moves);

  // Helper function that generates captures for pawns and kings.
  bool GenerateCaptures(int from,
                        int max_direction,
                        set<SevenTuple>* captures = NULL);

  // Stores the state of the board.
  Piece pieces[32];
};

#endif
