#include "catch.hpp"
#include "std.h"
#include "string-util.h"

TEST_CASE("String split three items", "[StringUtil]") {
  vector<string> tokens = StringUtil::Split("one,two,three", ',');
  REQUIRE(tokens.size() == 3);
  REQUIRE(tokens[0] == "one");
  REQUIRE(tokens[1] == "two");
  REQUIRE(tokens[2] == "three");
}

TEST_CASE("String split change delimiter", "[StringUtil]") {
  vector<string> tokens = StringUtil::Split("one|two| ,", '|');
  REQUIRE(tokens.size() == 3);
  REQUIRE(tokens[0] == "one");
  REQUIRE(tokens[1] == "two");
  REQUIRE(tokens[2] == " ,");
}

TEST_CASE("String split with only one item", "[StringUtil]") {
  vector<string> tokens = StringUtil::Split("word", ',');
  REQUIRE(tokens.size() == 1);
  REQUIRE(tokens[0] == "word");
}

TEST_CASE("String split with empty items", "[StringUtil]") {
  vector<string> tokens = StringUtil::Split(",,", ',');
  REQUIRE(tokens.size() == 3);
  REQUIRE(tokens[0] == "");
  REQUIRE(tokens[1] == "");
  REQUIRE(tokens[2] == "");
}

TEST_CASE("String split empty string", "[StringUtil]") {
  vector<string> tokens = StringUtil::Split("", ',');
  REQUIRE(tokens.size() == 1);
  REQUIRE(tokens[0] == "");
}

TEST_CASE("IsDigit", "[StringUtil]") {
  REQUIRE(StringUtil::IsDigit('0'));
  REQUIRE(StringUtil::IsDigit('1'));
  REQUIRE(StringUtil::IsDigit('5'));
  REQUIRE(StringUtil::IsDigit('8'));
  REQUIRE(StringUtil::IsDigit('9'));
  REQUIRE_FALSE(StringUtil::IsDigit('a'));
  REQUIRE_FALSE(StringUtil::IsDigit('Z'));
  REQUIRE_FALSE(StringUtil::IsDigit('/'));
  REQUIRE_FALSE(StringUtil::IsDigit(':'));
  REQUIRE_FALSE(StringUtil::IsDigit('+'));
}

TEST_CASE("DigitToInt", "[StringUtil]") {
  REQUIRE(StringUtil::DigitToInt('0') == 0);
  REQUIRE(StringUtil::DigitToInt('1') == 1);
  REQUIRE(StringUtil::DigitToInt('7') == 7);
  REQUIRE(StringUtil::DigitToInt('9') == 9);
}
