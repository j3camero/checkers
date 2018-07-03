#include "board.h"
#include "catch.hpp"

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
