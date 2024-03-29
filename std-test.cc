#include "catch.hpp"
#include "std.h"

TEST_CASE("Bit-shifting and endianness check", "[std]") {
  const uint64 eight = 8;
  REQUIRE((eight << 1) == 16);
  REQUIRE((eight >> 1) == 4);
  REQUIRE((eight >> 3) == 1);
  REQUIRE((eight >> 4) == 0);
  REQUIRE((eight >> 5) == 0);
}
