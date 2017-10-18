#include "catch.hpp"
#include "board.h"

TEST_CASE("Rename me 3", "[Board]") {
  Board b;
  REQUIRE(b == Board("   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   -   "
                     "   -   -   -   - "
                     " -   -   -   -   "));
}
