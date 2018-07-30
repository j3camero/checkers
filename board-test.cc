#include "board.h"
#include "catch.hpp"

#include "seven-tuple.h"
#include "six-tuple.h"

TEST_CASE("Get and Set", "[Board]") {
  Board b;
  REQUIRE(b.GetPiece(31) == Empty);
  b.SetPiece(31, BlackKing);
  REQUIRE(b.GetPiece(31) == BlackKing);
}

TEST_CASE("Human readable constructor", "[Board]") {
  Board b("   -   -   -   B "
          " -   -   -   -   "
          "   -   b   -   - "
          " -   w   -   -   "
          "   -   -   -   - "
          " -   -   -   -   "
          "   -   -   -   - "
          " W   -   -   -   ");
  REQUIRE(b.GetPiece(0) == WhiteKing);
  REQUIRE(b.GetPiece(1) == Empty);
  REQUIRE(b.GetPiece(17) == WhitePawn);
  REQUIRE(b.GetPiece(21) == BlackPawn);
  REQUIRE(b.GetPiece(31) == BlackKing);
}

TEST_CASE("Equality operator", "[Board]") {
  Board a;
  a.SetPiece(0, WhiteKing);
  a.SetPiece(1, Empty);
  a.SetPiece(17, WhitePawn);
  a.SetPiece(21, BlackPawn);
  a.SetPiece(31, BlackKing);
  Board b("   -   -   -   B "
          " -   -   -   -   "
          "   -   b   -   - "
          " -   w   -   -   "
          "   -   -   -   - "
          " -   -   -   -   "
          "   -   -   -   - "
          " W   -   -   -   ");
  REQUIRE(a == b);
  a.SetPiece(13, BlackPawn);
  b.SetPiece(13, WhitePawn);
  REQUIRE(a != b);
}

TEST_CASE("Move piece", "[Board]") {
  Board b;
  b.SetPiece(13, BlackPawn);
  b.MovePiece(13, 17);
  REQUIRE(b.GetPiece(13) == Empty);
  REQUIRE(b.GetPiece(17) == BlackPawn);
}

TEST_CASE("Trivial move", "[Board]") {
  Board b;
  b.SetPiece(13, BlackPawn);
  b.MovePiece(13, 13);
  REQUIRE(b.GetPiece(13) == BlackPawn);
}

TEST_CASE("Clear the board", "[Board]") {
  Board b;
  b.SetPiece(13, BlackPawn);
  b.SetPiece(17, WhiteKing);
  b.Clear();
  REQUIRE(b.GetPiece(13) == Empty);
  REQUIRE(b.GetPiece(17) == Empty);
}

TEST_CASE("Clear a specific type of piece from the board.", "[Board]") {
  Board a("   B   b   -   B "
          " w   W   -   -   "
          "   -   b   B   - "
          " b   w   -   -   "
          "   -   B   -   w "
          " -   W   B   -   "
          "   b   w   -   W "
          " W   -   b   -   ");
  a.Clear(BlackKing);
  Board b("   -   b   -   - "
          " w   W   -   -   "
          "   -   b   -   - "
          " b   w   -   -   "
          "   -   -   -   w "
          " -   W   -   -   "
          "   b   w   -   W "
          " W   -   b   -   ");
  REQUIRE(a == b);
}

TEST_CASE("Human readable output", "[Board]") {
  Board a("   B   b   -   B "
          " w   W   -   -   "
          "   -   b   B   - "
          " b   w   -   -   "
          "   -   B   -   w "
          " -   W   B   -   "
          "   b   w   -   W "
          " W   -   b   -   ");
  // The idea behind this test is to check that the same board can be
  // reconstructed from the human-readable output. It doesn't test whether
  // it really looks pretty or not, just proves that it at least contains the
  // right data.
  Board b(a.HumanReadable());
  REQUIRE(a == b);
}

TEST_CASE("WhichDatabaseSlice", "[Board]") {
  // Typical case. One of each piece type.
  REQUIRE(Board(
    "   -   -   -   B "
    " -   -   -   -   "
    "   -   b   -   - "
    " -   w   -   -   "
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " W   -   -   -   ").WhichDatabaseSlice() == SixTuple(1, 1, 1, 1, 5, 3));
  // Only kings.
  REQUIRE(Board(
    "   -   -   -   - "
    " -   -   -   -   "
    "   W   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   B   -   "
    "   -   -   -   - "
    " -   -   -   -   ").WhichDatabaseSlice() == SixTuple(1, 1, 0, 0, 0, 0));
  // Only pawns.
  REQUIRE(Board(
    "   -   -   -   - "
    " -   -   -   -   "
    "   w   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   b   -   "
    "   -   -   -   - "
    " -   -   -   -   ").WhichDatabaseSlice() == SixTuple(0, 0, 1, 1, 2, 2));
  // Many pieces.
  REQUIRE(Board(
    "   -   B   W   - "
    " -   b   -   B   "
    "   w   -   -   - "
    " -   -   W   -   "
    "   -   W   -   - "
    " -   -   -   w   "
    "   -   B   -   - "
    " -   W   -   -   ").WhichDatabaseSlice() == SixTuple(3, 4, 1, 2, 6, 5));
}

TEST_CASE("Index of Board", "[Board]") {
  SevenTuple s = Board("   -   -   -   - "
                       " -   -   -   -   "
                       "   -   -   -   - "
                       " w   -   -   -   "
                       "   -   -   -   - "
                       " -   -   -   b   "
                       "   -   -   -   - "
                       " -   -   -   -   ").Index();
  REQUIRE(s.GetDB() == SixTuple(0, 0, 1, 1, 2, 3));
  REQUIRE(s.GetIndex() == 0);
  // Two non-overlapping pawns.
  REQUIRE(Board("   -   -   -   - "
                " -   -   -   -   "
                "   -   -   -   - "
                " -   w   -   -   "
                "   -   -   -   - "
                " -   -   -   b   "
                "   -   -   -   - "
                " -   -   -   -   ").Index().GetIndex() == 1);
  REQUIRE(Board("   -   -   -   - "
                " -   -   -   -   "
                "   -   -   -   - "
                " -   -   -   w   "
                "   -   -   -   - "
                " b   -   -   -   "
                "   -   -   -   - "
                " -   -   -   -   ").Index().GetIndex() == 15);
  // Two overlapping pawns.
  REQUIRE(Board("   -   -   -   - "
                " -   -   -   -   "
                "   -   -   -   - "
                " w   -   -   b   "
                "   -   -   -   - "
                " -   -   -   -   "
                "   -   -   -   - "
                " -   -   -   -   ").Index().GetIndex() == 0);
  REQUIRE(Board("   -   -   -   - "
                " -   -   -   -   "
                "   -   -   -   - "
                " -   w   -   b   "
                "   -   -   -   - "
                " -   -   -   -   "
                "   -   -   -   - "
                " -   -   -   -   ").Index().GetIndex() == 1);
  REQUIRE(Board("   -   -   -   - "
                " -   -   -   -   "
                "   -   -   -   - "
                " b   -   -   w   "
                "   -   -   -   - "
                " -   -   -   -   "
                "   -   -   -   - "
                " -   -   -   -   ").Index().GetIndex() == 11);
  // All four piece types.
  REQUIRE(Board("   -   -   -   - "
                " -   -   -   -   "
                "   w   -   -   - "
                " -   -   -   -   "
                "   -   -   -   - "
                " -   -   -   b   "
                "   -   -   -   - "
                " B   W   -   -   ").Index().GetIndex() == 0);
  REQUIRE(Board("   -   -   -   - "
                " -   -   -   -   "
                "   w   -   -   - "
                " -   -   -   -   "
                "   -   -   -   - "
                " -   -   -   b   "
                "   -   -   -   - "
                " B   -   W   -   ").Index().GetIndex() == 1);
  REQUIRE(Board("   -   W   -   B "
                " -   -   -   -   "
                "   -   -   -   w "
                " -   -   -   -   "
                "   -   -   -   - "
                " b   -   -   -   "
                "   -   -   -   - "
                " -   -   -   -   ").Index().GetIndex() == 13918);
  REQUIRE(Board("   -   -   W   B "
                " -   -   -   -   "
                "   -   -   -   w "
                " -   -   -   -   "
                "   -   -   -   - "
                " b   -   -   -   "
                "   -   -   -   - "
                " -   -   -   -   ").Index().GetIndex() == 13919);
  // Two black pawns, one white pawn.
  REQUIRE(Board(
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " w   -   -   -   "
    "   -   -   -   - "
    " -   -   b   b   "
    "   -   -   -   - "
    " -   -   -   -   ").Index().GetIndex() == 0);
  REQUIRE(Board(
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   w   -   -   "
    "   -   -   -   - "
    " -   -   b   b   "
    "   -   -   -   - "
    " -   -   -   -   ").Index().GetIndex() == 1);
  REQUIRE(Board(
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " w   -   -   -   "
    "   -   -   -   - "
    " -   b   -   b   "
    "   -   -   -   - "
    " -   -   -   -   ").Index().GetIndex() == 4);
  REQUIRE(Board(
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   w   "
    "   -   -   -   - "
    " b   -   -   b   "
    "   -   -   -   - "
    " -   -   -   -   ").Index().GetIndex() == 11);
  REQUIRE(Board(
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " w   -   -   -   "
    "   -   -   -   - "
    " -   -   -   b   "
    "   -   -   -   b "
    " -   -   -   -   ").Index().GetIndex() == 12);
  REQUIRE(Board(
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " w   -   -   -   "
    "   -   -   -   - "
    " -   -   -   b   "
    "   -   -   -   - "
    " b   -   -   -   ").Index().GetIndex() == 40);
  REQUIRE(Board(
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   w   "
    "   -   -   -   - "
    " -   -   -   b   "
    "   -   -   -   - "
    " b   -   -   -   ").Index().GetIndex() == 43);
  REQUIRE(Board(
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " w   -   -   -   "
    "   -   -   -   - "
    " -   b   b   -   "
    "   -   -   -   - "
    " -   -   -   -   ").Index().GetIndex() == 44);
  REQUIRE(Board(
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   w   -   "
    "   -   -   -   - "
    " b   -   -   -   "
    "   -   -   -   - "
    " b   -   -   -   ").Index().GetIndex() == 150);
  REQUIRE(Board(
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   w   "
    "   -   -   -   - "
    " b   -   -   -   "
    "   -   -   -   - "
    " b   -   -   -   ").Index().GetIndex() == 151);
  // Two of each piece type.
  REQUIRE(Board(
    "   w   w   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   -   "
    "   W   W   -   - "
    " B   B   b   b   ").Index().GetIndex() == 0);
  REQUIRE(Board(
    "   w   w   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   -   "
    "   W   -   W   - "
    " B   B   b   b   ").Index().GetIndex() == 1);
  REQUIRE(Board(
    "   w   w   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   W   W   - "
    " B   B   b   b   ").Index().GetIndex() == 25);
  REQUIRE(Board(
    "   w   w   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   -   "
    "   B   W   -   - "
    " B   W   b   b   ").Index().GetIndex() == 325);
  REQUIRE(Board(
    "   w   -  w   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   -   "
    "   W   W   -   - "
    " B   B   b   b   ").Index().GetIndex() == 122850);
  REQUIRE(Board(
    "   -   w   w   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   -   "
    "   W   W   -   - "
    " B   B   b   b   ").Index().GetIndex() == 3 * 122850);
  REQUIRE(Board(
    "   w   w   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   -   "
    "   W   W   -   - "
    " B   b   B   b   ").Index().GetIndex() == 6 * 122850);
  REQUIRE(Board(
    "   B   B   w   w "
    " -   -   W   W   "
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " b   b   -   -   ").Index().GetIndex() == 36 * 122850 - 1);
}

TEST_CASE("Deindex Board, hand-calculated examples", "[Board]") {
  // Two non-overlapping pawns.
  REQUIRE(Board(0, 0, 1, 1, 2, 3, 0) == Board(
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " w   -   -   -   "
    "   -   -   -   - "
    " -   -   -   b   "
    "   -   -   -   - "
    " -   -   -   -   "
  ));
  REQUIRE(Board(0, 0, 1, 1, 2, 3, 1) == Board(
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   w   -   -   "
    "   -   -   -   - "
    " -   -   -   b   "
    "   -   -   -   - "
    " -   -   -   -   "
  ));
  REQUIRE(Board(0, 0, 1, 1, 2, 3, 15) == Board(
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   w   "
    "   -   -   -   - "
    " b   -   -   -   "
    "   -   -   -   - "
    " -   -   -   -   "
  ));
  // Two overlapping pawns.
  REQUIRE(Board(0, 0, 1, 1, 4, 3, 0) == Board(
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " w   -   -   b   "
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   -   "
  ));
  REQUIRE(Board(0, 0, 1, 1, 4, 3, 1) == Board(
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   w   -   b   "
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   -   "
  ));
  REQUIRE(Board(0, 0, 1, 1, 4, 3, 11) == Board(
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " b   -   -   w   "
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   -   "
  ));
  // All four piece types.
  REQUIRE(Board(1, 1, 1, 1, 2, 2, 0) == Board(
    "   -   -   -   - "
    " -   -   -   -   "
    "   w   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   b   "
    "   -   -   -   - "
    " B   W   -   -   "
  ));
  REQUIRE(Board(1, 1, 1, 1, 2, 2, 1) == Board(
    "   -   -   -   - "
    " -   -   -   -   "
    "   w   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   b   "
    "   -   -   -   - "
    " B   -   W   -   "
  ));
  REQUIRE(Board(1, 1, 1, 1, 2, 2, 13918) == Board(
    "   -   W   -   B "
    " -   -   -   -   "
    "   -   -   -   w "
    " -   -   -   -   "
    "   -   -   -   - "
    " b   -   -   -   "
    "   -   -   -   - "
    " -   -   -   -   "
  ));
  REQUIRE(Board(1, 1, 1, 1, 2, 2, 13919) == Board(
    "   -   -   W   B "
    " -   -   -   -   "
    "   -   -   -   w "
    " -   -   -   -   "
    "   -   -   -   - "
    " b   -   -   -   "
    "   -   -   -   - "
    " -   -   -   -   "
  ));
  // Two black pawns, one white pawn.
  REQUIRE(Board(0, 0, 2, 1, 2, 3, 0) == Board(
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " w   -   -   -   "
    "   -   -   -   - "
    " -   -   b   b   "
    "   -   -   -   - "
    " -   -   -   -   "
  ));
  REQUIRE(Board(0, 0, 2, 1, 2, 3, 1) == Board(
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   w   -   -   "
    "   -   -   -   - "
    " -   -   b   b   "
    "   -   -   -   - "
    " -   -   -   -   "
  ));
  REQUIRE(Board(0, 0, 2, 1, 2, 3, 4) == Board(
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " w   -   -   -   "
    "   -   -   -   - "
    " -   b   -   b   "
    "   -   -   -   - "
    " -   -   -   -   "
  ));
  REQUIRE(Board(0, 0, 2, 1, 2, 3, 11) == Board(
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   w   "
    "   -   -   -   - "
    " b   -   -   b   "
    "   -   -   -   - "
    " -   -   -   -   "
  ));
  REQUIRE(Board(0, 0, 2, 1, 2, 3, 12) == Board(
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " w   -   -   -   "
    "   -   -   -   - "
    " -   -   -   b   "
    "   -   -   -   b "
    " -   -   -   -   "
  ));
  REQUIRE(Board(0, 0, 2, 1, 2, 3, 40) == Board(
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " w   -   -   -   "
    "   -   -   -   - "
    " -   -   -   b   "
    "   -   -   -   - "
    " b   -   -   -   "
  ));
  REQUIRE(Board(0, 0, 2, 1, 2, 3, 43) == Board(
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   w   "
    "   -   -   -   - "
    " -   -   -   b   "
    "   -   -   -   - "
    " b   -   -   -   "
  ));
  REQUIRE(Board(0, 0, 2, 1, 2, 3, 44) == Board(
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " w   -   -   -   "
    "   -   -   -   - "
    " -   b   b   -   "
    "   -   -   -   - "
    " -   -   -   -   "
  ));
  REQUIRE(Board(0, 0, 2, 1, 2, 3, 150) == Board(
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   w   -   "
    "   -   -   -   - "
    " b   -   -   -   "
    "   -   -   -   - "
    " b   -   -   -   "
  ));
  REQUIRE(Board(0, 0, 2, 1, 2, 3, 151) == Board(
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   w   "
    "   -   -   -   - "
    " b   -   -   -   "
    "   -   -   -   - "
    " b   -   -   -   "
  ));
  // Two of each piece type.
  REQUIRE(Board(2, 2, 2, 2, 0, 0, 0) == Board(
    "   w   w   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   -   "
    "   W   W   -   - "
    " B   B   b   b   "
  ));
  REQUIRE(Board(2, 2, 2, 2, 0, 0, 1) == Board(
    "   w   w   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   -   "
    "   W   -   W   - "
    " B   B   b   b   "
  ));
  REQUIRE(Board(2, 2, 2, 2, 0, 0, 25) == Board(
    "   w   w   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   W   W   - "
    " B   B   b   b   "
  ));
  REQUIRE(Board(2, 2, 2, 2, 0, 0, 325) == Board(
    "   w   w   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   -   "
    "   B   W   -   - "
    " B   W   b   b   "
  ));
  REQUIRE(Board(2, 2, 2, 2, 0, 0, 122850) == Board(
    "   w   -  w   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   -   "
    "   W   W   -   - "
    " B   B   b   b   "
  ));
  REQUIRE(Board(2, 2, 2, 2, 0, 0, 3 * 122850) == Board(
    "   -   w   w   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   -   "
    "   W   W   -   - "
    " B   B   b   b   "
  ));
  REQUIRE(Board(2, 2, 2, 2, 0, 0, 6 * 122850) == Board(
    "   w   w   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   -   "
    "   W   W   -   - "
    " B   b   B   b   "
  ));
  REQUIRE(Board(2, 2, 2, 2, 0, 0, 36 * 122850 - 1) == Board(
    "   B   B   w   w "
    " -   -   W   W   "
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " b   b   -   -   "
  ));
}

TEST_CASE("Index and Deindex are consistent, 4M+", "[Board]") {
  // Choose some indices from a database slice that has over 4M positions.
  // Deindex then re-index them to make sure the index comes out the same.
  SixTuple db(2, 2, 2, 2, 0, 0);
  for (uint64 index = 11; index < 4 * 1000 * 1000; index += 400007) {
    REQUIRE(Board(db, index).Index() == SevenTuple(db, index));
  }
}

TEST_CASE("Index and Deindex are consistent, 28M+", "[Board]") {
  // Choose some indices from a database slice that has over 28M positions.
  // Deindex then re-index them to make sure the index comes out the same.
  SixTuple db(3, 2, 1, 2, 0, 0);
  for (uint64 index = 11; index < 28 * 1000 * 1000; index += 1000007) {
    REQUIRE(Board(db, index).Index() == SevenTuple(db, index));
  }
}

TEST_CASE("Mirror of board position.", "[Board]") {
  REQUIRE(
    Board("   B   -   -   - "
          " -   -   w   b   "
          "   W   -   w   - "
          " -   -   b   w   "
          "   -   -   -   - "
          " -   b   B   -   "
          "   -   -   -   B "
          " b   -   -   -   ").Mirror() ==
    Board("   -   -   -   w "
          " W   -   -   -   "
          "   -   W   w   - "
          " -   -   -   -   "
          "   b   w   -   - "
          " -   b   -   B   "
          "   w   b   -   - "
          " -   -   -   W   ")
  );
}

TEST_CASE("MirrorIndex hand calculated examples.", "[Board]") {
  REQUIRE(Board(
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " w   -   -   -   "
    "   -   -   -   - "
    " -   -   -   b   "
    "   -   -   -   - "
    " -   -   -   -   ").MirrorIndex().GetIndex() == 0);
  REQUIRE(Board(
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " w   -   -   -   "
    "   -   -   -   - "
    " -   -   b   -   "
    "   -   -   -   - "
    " -   -   -   -   ").MirrorIndex().GetIndex() == 1);
  REQUIRE(Board(
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   w   "
    "   -   -   -   - "
    " b   -   -   -   "
    "   -   -   -   - "
    " -   -   -   -   ").MirrorIndex().GetIndex() == 15);
  REQUIRE(Board(
    "   -   -   B   W "
    " -   -   -   -   "
    "   -   -   -   - "
    " w   -   -   b   "
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   -   ").MirrorIndex().GetIndex() == 0);
  REQUIRE(Board(
    "   -   B   -   W "
    " -   -   -   -   "
    "   -   -   -   - "
    " w   -   -   b   "
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " -   -   -   -   ").MirrorIndex().GetIndex() == 1);
  REQUIRE(Board(
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " b   -   -   w   "
    "   -   -   -   - "
    " -   -   -   -   "
    "   -   -   -   - "
    " W   B   -   -   ").MirrorIndex().GetIndex() == 10439);
}

TEST_CASE("MirrorIndex is self consistent, 28M+", "[Board]") {
  // Choose some indices from a database slice that has over 28M positions.
  // Check that .MirrorIndex() matches .Mirror().Index().
  SixTuple db(3, 2, 1, 2, 0, 0);
  for (uint64 index = 13; index < 28 * 1000 * 1000; index += 1000007) {
    Board b(db, index);
    REQUIRE(b.MirrorIndex() == b.Mirror().Index());
  }
}
