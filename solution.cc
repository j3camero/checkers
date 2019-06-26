#include "solution.h"
#include "std.h"

// Section 1 of 3 (Small).

SmallSolutionBuffer::SmallSolutionBuffer() : data(0) {
  // Body is empty because of initialization list.
}

SmallSolutionBuffer::SmallSolutionBuffer(const SmallSolutionBuffer& other)
  : data(other.data) {
  // Body is empty because of initialization list.
}

Solution SmallSolutionBuffer::Get(int index) const {
  const int bit_offset = 2 * index;
  const Solution value = (Solution)((data >> bit_offset) & 3);
  return value;
}

void SmallSolutionBuffer::Set(int index, Solution value) {
  const int bit_offset = 2 * index;
  const uint64 cleared_slot = data & ~(3 << bit_offset);
  const uint64 shifted_value = ((uint64)((int)value)) << bit_offset;
  data = shifted_value | cleared_slot;
}

// Section 2 of 3 (Medium).

MediumSolutionBuffer::MediumSolutionBuffer(uint64 length)
  : length(length), data(length / 32 + 1) {
}

MediumSolutionBuffer::MediumSolutionBuffer(const string& filename) {
  bool success = Read(filename);
  if (!success) {
    throw "Failed to read from file.";
  }
}

Solution MediumSolutionBuffer::Get(uint64 index) const {
  const uint64 slot_index = index / 32;
  const SmallSolutionBuffer& slot = data[slot_index];
  const int index_within_slot = index % 32;
  return slot.Get(index_within_slot);
}

void MediumSolutionBuffer::Set(uint64 index, Solution value) {
  const uint64 slot_index = index / 32;
  SmallSolutionBuffer& slot = data[slot_index];
  const int index_within_slot = index % 32;
  slot.Set(index_within_slot, value);
}

uint64 MediumSolutionBuffer::Length() {
  return length;
}

bool MediumSolutionBuffer::Write(const string& filename) const {
  ofstream file(filename.c_str(), ios::out | ios::binary);
  const uint64 version = 1;
  file.write((char*)&version, sizeof version);
  file.write((char*)&length, sizeof length);
  for (uint64 i = 0; i < data.size(); ++i) {
    file.write((char*)&data[i], sizeof data[i]);
  }
  file.close();
  return true;
}

bool MediumSolutionBuffer::Read(const string& filename) {
  ifstream file(filename.c_str(), ios::in | ios::binary);
  uint64 version;
  file.read((char*)&version, sizeof version);
  file.read((char*)&length, sizeof length);
  data.resize(length / 32 + 1);
  for (uint64 i = 0; i < data.size(); ++i) {
    file.read((char*)&data[i], sizeof data[i]);
  }
  file.close();
  return true;
}

// Section 3 of 3 (Large).

const int max_page_bytes = 64 * 1024;
const int max_page_length = 4 * max_page_bytes;

LargeSolutionBuffer::LargeSolutionBuffer(uint64 length)
  : length(length),
    data(length / max_page_length, MediumSolutionBuffer(max_page_length)) {
  const uint64 remaining_length = length % max_page_length;
  if (remaining_length > 0) {
    data.push_back(MediumSolutionBuffer(remaining_length));
  }
}

Solution LargeSolutionBuffer::Get(uint64 index) {
  const int which_page = index / max_page_length;
  const MediumSolutionBuffer& page = data[which_page];
  const uint64 index_within_page = index % max_page_length;
  return page.Get(index_within_page);
}

void LargeSolutionBuffer::Set(uint64 index, Solution value) {
  const int which_page = index / max_page_length;
  MediumSolutionBuffer& page = data[which_page];
  const uint64 index_within_page = index % max_page_length;
  page.Set(index_within_page, value);
}

uint64 LargeSolutionBuffer::Length() {
  return length;
}
