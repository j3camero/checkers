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
  Bitstring();
  Bitstring(bool b);
  Bitstring(uint64 length);
  Bitstring(const Bitstring& b);

  bool Get(uint64 index) const;
  void Set(uint64 index, bool b);

  uint64 Size() const;
  void Resize(uint64 new_size);

  void Append(bool b);
  void Append(const Bitstring& b);

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
