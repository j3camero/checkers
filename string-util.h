#ifndef _STRING_UTIL_H_
#define _STRING_UTIL_H_

#include "std.h"

// Helper functions for C++ strings.
class StringUtil {
 public:
  // Split s into tokens delimited by delim.
  static vector<string> Split(const string& s, char delim);

  // Returns true if c is an ASCII digit like '0' or '9' and false otherwise.
  static bool IsDigit(char c);

  // Converts an ASCII character like '7' to the equivalent int 7.
  static int DigitToInt(char c);
};

#endif
