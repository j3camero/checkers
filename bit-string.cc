#include "bit-string.h"

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
  return false;
}

std::ostream& operator<<(std::ostream &out, const Bitstring& b) {
  for (uint64 i = 0; i < b.Size(); ++i) {
    if (b.Get(i)) {
      out << "+";
    } else {
      out << "-";
    }
  }
  return out;
}
