#include "medium-solution-buffer.h"
#include "small-solution-buffer.h"
#include "solution.h"
#include "std.h"

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
