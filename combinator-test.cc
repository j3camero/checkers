#include "board.h"
#include "catch.hpp"
#include "combinator.h"

TEST_CASE("Constructor", "[Combinator]") {
  Combinator c(3, 2);
  REQUIRE(c.GetN() == 3);
  REQUIRE(c.GetK() == 2);
  REQUIRE(c.GetCounter(0) == 0);
  REQUIRE(c.GetCounter(1) == 1);
  REQUIRE(c.Index() == 0);
}

TEST_CASE("Minimal combinators", "[Combinator]") {
  Combinator zero(1, 0);
  REQUIRE(zero.Index() == 0);
  REQUIRE(zero.Increment());
  REQUIRE(zero.Index() == 0);

  Combinator one(1, 1);
  REQUIRE(one.Index() == 0);
  REQUIRE(one.Increment());
  REQUIRE(one.Index() == 0);
  REQUIRE(one.GetCounter(0) == 0);

  Combinator two(2, 1);
  REQUIRE(two.Index() == 0);
  REQUIRE(two.GetCounter(0) == 0);
  REQUIRE_FALSE(two.Increment());
  REQUIRE(two.Index() == 1);
  REQUIRE(two.GetCounter(0) == 1);
  REQUIRE(two.Increment());
  REQUIRE(two.Index() == 0);
  REQUIRE(two.GetCounter(0) == 0);
}

TEST_CASE("Small combinator overflow", "[Combinator]") {
  Combinator c(3, 2);  // This combinator has 3 states.
  REQUIRE_FALSE(c.Increment());  // So increment it two times.
  REQUIRE_FALSE(c.Increment());
  REQUIRE(c.GetCounter(0) == 1);  // Check the state before overflow.
  REQUIRE(c.GetCounter(1) == 2);
  REQUIRE(c.Index() == 2);
  REQUIRE(c.Increment());  // Overflow the combinator.
  REQUIRE(c.GetCounter(0) == 0);  // Check the state after overflow.
  REQUIRE(c.GetCounter(1) == 1);
  REQUIRE(c.Index() == 0);  // Overflow causes the combinator to be reset.
}

TEST_CASE("Medium combinator overflow", "[Combinator]") {
  // 5 choose 3 has 10 different states before it overflows.
  Combinator c(5, 3);
  REQUIRE_FALSE(c.Increment(9));
  REQUIRE(c.GetCounter(0) == 2);
  REQUIRE(c.GetCounter(2) == 4);
  REQUIRE(c.Increment());  // Overflow.
  REQUIRE(c.Index() == 0);
  REQUIRE(c.GetCounter(0) == 0);
  REQUIRE(c.GetCounter(2) == 2);
}

TEST_CASE("Large combinator overflow", "[Combinator]") {
  // 28 choose 5 has 98,280 different states before it overflows.
  Combinator c(28, 5);
  REQUIRE_FALSE(c.Increment(98279));
  REQUIRE(c.GetCounter(0) == 23);
  REQUIRE(c.GetCounter(4) == 27);
  REQUIRE(c.Increment());  // Overflow.
  REQUIRE(c.Index() == 0);
  REQUIRE(c.GetCounter(0) == 0);
  REQUIRE(c.GetCounter(4) == 4);
}

TEST_CASE("Multiples overflows", "[Combinator]") {
  // Use a combinator that has 2 states to test that multiple overflows work.
  Combinator c(2, 1);
  REQUIRE_FALSE(c.Increment());
  REQUIRE(c.Increment());  // One.
  REQUIRE_FALSE(c.Increment());
  REQUIRE(c.Increment());  // Two.
  REQUIRE_FALSE(c.Increment());
  REQUIRE(c.Increment()); // Three.
}

TEST_CASE("Move 1 piece on a checkerboard, no permutation", "[Combinator]") {
  // Use a trivial permutation 0->0, 1->1, and 2->2.
  std::vector<int> permutation;
  permutation.push_back(0);
  permutation.push_back(1);
  permutation.push_back(2);
  Board b("   -   -   -   - "
          " -   -   -   -   "
          "   -   -   -   - "
          " -   -   -   -   "
          "   -   -   -   - "
          " -   -   -   -   "
          "   -   -   -   - "
          " b   -   -   -   ");
  Combinator c(3, 1);
  REQUIRE_FALSE(c.Increment(&permutation, &b));
  REQUIRE(b == Board("   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   b   -   -   "));
  REQUIRE_FALSE(c.Increment(&permutation, &b));
  REQUIRE(b == Board("   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   b   -   "));
  REQUIRE(c.Increment(&permutation, &b));
  REQUIRE(b == Board("   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " b   -   -   -   "));
}

TEST_CASE("Two checkers dance on a checkerboard together", "[Combinator]") {
  std::vector<int> permutation;
  // Three diagonally connected squares.
  permutation.push_back(22);
  permutation.push_back(18);
  permutation.push_back(13);
  Board b("   -   -   -   - "
          " -   -   -   -   "
          "   -   -   B   - "
          " -   -   w   -   "
          "   -   -   -   - "
          " -   -   -   -   "
          "   -   -   -   - "
          " -   -   -   -   ");
  Combinator c(3, 2);
  REQUIRE_FALSE(c.Increment(&permutation, &b));
  REQUIRE(b == Board("   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   B   - "
                     " -   -   -   -   "
                     "   -   w   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   -   "));
  REQUIRE_FALSE(c.Increment(&permutation, &b));
  REQUIRE(b == Board("   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   B   -   "
                     "   -   w   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   -   "));
  REQUIRE(c.Increment(&permutation, &b));
  REQUIRE(b == Board("   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   B   - "
                     " -   -   w   -   "
                     "   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   -   "));
}

TEST_CASE("Move 3 pieces on a checkerboard with permutation", "[Combinator]") {
  std::vector<int> permutation;
  // The first 4 ranks in reverse order.
  for (int i = 15; i >= 0; --i) {
    permutation.push_back(i);
  }
  Board b("   -   -   -   - "
          " -   -   -   -   "
          "   -   -   -   - "
          " -   -   -   -   "
          "   -   B   W   b "
          " -   -   -   -   "
          "   -   -   -   - "
          " -   -   -   -   ");
  Combinator c(16, 3);
  REQUIRE_FALSE(c.Increment(559, &permutation, &b));
  REQUIRE(b == Board("   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " B   W   b   -   "));
  REQUIRE(c.Increment(&permutation, &b));
  REQUIRE(b == Board("   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   -   "
                     "   -   B   W   b "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   -   "));
}