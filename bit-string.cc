#include "bit-string.h"

#include <iostream>

Bitstring::Bitstring() : size(0) {
  // Do nothing.
}

Bitstring::Bitstring(bool b) : size(0) {
  Append(b);
}

Bitstring::Bitstring(uint64 length) : size(0) {
  Resize(length);
}

Bitstring::Bitstring(const Bitstring& b) : size(0) {
  Append(b);
}

Bitstring::Bitstring(const std::string& s) : size(0) {
  Append(s);
}

Bitstring::Bitstring(const char *s) : size(0) {
  Append(std::string(s));
}

bool Bitstring::Get(uint64 index) const {
  const int page_index = index / bits_per_page;
  const Page& page = data[page_index];
  const int index_within_page = index % bits_per_page;
  const int word_index = index_within_page / bits_per_page;
  const uint64 word = page[word_index];
  const int index_within_word = index_within_page % 64;
  return (word >> index_within_word) & 1;
}

void Bitstring::Set(uint64 index, bool b) {
  const int page_index = index / bits_per_page;
  Page& page = data[page_index];
  const int index_within_page = index % bits_per_page;
  const int word_index = index_within_page / bits_per_page;
  const int index_within_word = index_within_page % 64;
  if (b) {
    // Set the bit.
    page[word_index] |= uint64(1) << index_within_word;
  } else {
    // Clear the bit.
    page[word_index] &= ~(uint64(1) << index_within_word);
  }
}

void Bitstring::Clear(uint64 index) {
  Set(index, false);
}

void Bitstring::Flip(uint64 index) {
  Set(index, !Get(index));
}

uint64 Bitstring::Size() const {
  return size;
}

void Bitstring::Resize(uint64 new_size) {
  size = new_size;
  // Add or remove pages.
  const int new_page_count = size / bits_per_page + 1;
  const Page blank_page(words_per_page);
  data.resize(new_page_count, blank_page);
  // Change the size of the last page.
  const int last_page_bits = size % bits_per_page;
  const int new_word_count = last_page_bits / 64 + 1;
  Page& last_page = data.back();
  last_page.resize(new_word_count);
}

void Bitstring::Append(bool b) {
  const uint64 i = size;
  Resize(i + 1);
  Set(i, b);
}

void Bitstring::Append(const Bitstring& b) {
  for (uint64 i = 0; i < b.Size(); ++i) {
    bool bit = b.Get(i);
    Append(bit);
  }
}

void Bitstring::Append(const std::string& s) {
  // Process digits in right to left order in accordance with math convention.
  for (int i = s.size() - 1; i >= 0; --i) {
    char c = s[i];
    Append(c);
  }
}

void Bitstring::Append(char c) {
  switch (c) {
    case '0':
    case 'F':
    case 'f':
    case '-':
      Append(false);
      break;
    case '1':
    case 'T':
    case 't':
    case '+':
      Append(true);
      break;
  };
}

uint64 Bitstring::ToUInt64() {
  if (size > 64) {
    return 0;
  }
  uint64 sum = 0;
  uint64 mag = 1;
  for (uint64 i = 0; i < size; ++i) {
    if (Get(i)) {
      sum += mag;
    }
    mag *= 2;
  }
  return sum;
}

void Bitstring::Increment() {
  bool done = false;
  for (uint64 i = 0; i < size; ++i) {
    if (Get(i)) {
      Clear(i);
    } else {
      Set(i);
      done = true;
      break;
    }
  }
  if (!done) {
    Append(true);
  }
}

bool Bitstring::operator==(const Bitstring& b) const {
  if (b.size != size) {
    return false;
  }
  for (uint64 i = 0; i < size; ++i) {
    if (Get(i) != b.Get(i)) {
      return false;
    }
  }
  return true;
}

bool Bitstring::operator!=(const Bitstring& b) const {
  return !(*this == b);
}

bool Bitstring::operator<(const Bitstring& b) const {
  if (size < b.Size()) {
    return true;
  }
  if (b.Size() < size) {
    return false;
  }
  if (size == 0) {
    // This case can't be handled nicely by the loop below, so it is
    // handled individually here.
    return false;
  }
  for (uint64 i = size - 1; i >= 0; --i) {
    if (Get(i)) {
      if (!b.Get(i)) {
        return false;
      }
    } else {
      if (b.Get(i)) {
        return true;
      }
    }
    // Needs to be here because the loop condition doesn't work.
    if (i == 0) {
      break;
    }
  }
  return false;
}

Bitstring& Bitstring::operator+=(const Bitstring& b) {
  Append(b);
  return *this;
}

Bitstring operator+(const Bitstring& a, const Bitstring& b) {
  // Make a copy of a.
  Bitstring c(a);
  // Append b to the copy.
  c.Append(b);
  return c;
}

std::ostream& operator<<(std::ostream &out, const Bitstring& b) {
  for (uint64 i = b.Size(); i > 0; --i) {
    if (b.Get(i - 1)) {
      out << "+";
    } else {
      out << "-";
    }
  }
  return out;
}
