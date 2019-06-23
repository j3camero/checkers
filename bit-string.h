#ifndef _BITSTRING_H_
#define _BITSTRING_H_

#include <string>
#include <vector>

#include "types.h"

// A paging scheme is used to break up the memory allocations.
const int words_per_page = 1024;
const int bits_per_page = words_per_page * 64;
typedef std::vector<uint64> Page;

// A string of bits.
class Bitstring {
 public:
  // Makes an empty bit string with length zero.
  Bitstring();

  // Makes a bit string with 1 bit.
  Bitstring(bool b);

  // Makes a bit string with the given length, initialized to all zeros.
  Bitstring(uint64 length);

  // Copy constructor.
  Bitstring(const Bitstring& b);

  // Construct from a string. Useful for short Bitstring literals.
  Bitstring(const std::string& s);
  Bitstring(const char *s);

  // Set and get individual bits.
  bool Get(uint64 index) const;
  void Set(uint64 index, bool b);

  // How many bits in this bit string.
  uint64 Size() const;

  // Resizes the bit string to a new length.
  void Resize(uint64 new_size);

  // Add one bit to the end of the string.
  void Append(bool b);

  // Add some bits from b to the end of this string.
  void Append(const Bitstring& b);

  // Add bits from a human-readable string.
  void Append(const std::string& s);

  // Interpret one ASCII character as a bit and add it.
  void Append(char c);

  // For short bit strings only. Interprets the bits as a number.
  uint64 ToUInt64();

  // Operators.
  bool operator==(const Bitstring& b) const;
  bool operator!=(const Bitstring& b) const;
  bool operator<(const Bitstring& b) const;

  // Outputs the Bitstring to a stream as a sequence of + and - characters.
  friend std::ostream& operator<<(std::ostream &out, const Bitstring& b);

 private:
  // Store data as "list of lists" to break up the memory allocations.
  std::vector<Page> data;

  // The size of the bit string, in bits.
  uint64 size;
};

#endif
