#include "medium-solution-buffer.h"

#include <fstream>

MediumSolutionBuffer::MediumSolutionBuffer(uint64 length)
  : length(length), data(length / 32 + 1, 0) {
}

MediumSolutionBuffer::MediumSolutionBuffer(const std::string& filename) {
  bool success = Read(filename);
  if (!success) {
    throw "Failed to read from file.";
  }
}

Solution MediumSolutionBuffer::Get(uint64 index) {
  const uint64 slot_index = index / 32;
  const uint64 slot = data[slot_index];
  const int index_within_slot = index % 32;
  const int bit_offset = 2 * index_within_slot;
  const Solution value = (Solution)((slot >> bit_offset) & 3);
  return value;
}

void MediumSolutionBuffer::Set(uint64 index, Solution value) {
  const uint64 slot_index = index / 32;
  const uint64 slot = data[slot_index];
  const int index_within_slot = index % 32;
  const int bit_offset = 2 * index_within_slot;
  const uint64 cleared_slot = slot & ~(3 << bit_offset);
  const uint64 shifted_value = ((uint64)((int)value)) << bit_offset;
  data[slot_index] = shifted_value | cleared_slot;
}

uint64 MediumSolutionBuffer::Length() {
  return length;
}

bool MediumSolutionBuffer::Write(const std::string& filename) {
  std::ofstream file(filename.c_str(), std::ios::out | std::ios::binary);
  const uint64 version = 1;
  file.write((char*)&version, sizeof version);
  file.write((char*)&length, sizeof length);
  for (uint64 i = 0; i < data.size(); ++i) {
    file.write((char*)&data[i], sizeof data[i]);
  }
  file.close();
  return true;
}

bool MediumSolutionBuffer::Read(const std::string& filename) {
  std::ifstream file(filename.c_str(), std::ios::in | std::ios::binary);
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
