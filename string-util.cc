#include "std.h"
#include "string-util.h"

vector<string> StringUtil::Split(const string& s, char delim) {
  vector<string> tokens;
  string token;
  for (int i = 0; i < s.size(); ++i) {
    if (s[i] == delim) {
      tokens.push_back(token);
      token.clear();
    } else {
      token += s[i];
    }
  }
  tokens.push_back(token);
  return tokens;
}

bool StringUtil::IsDigit(char c) {
  return c >= '0' && c <= '9';
}

int StringUtil::DigitToInt(char c) {
  return int(c) - int('0');
}
