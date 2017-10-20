#include "solution-buffer.h"

#include <fstream>

SolutionBuffer::SolutionBuffer(uint64 _length) {
  length = _length;
  uint64 buffer_slots = length / 32;
  if (length % 32 != 0) {
    buffer_slots += 1;
  }
  data = new uint64[buffer_slots];
  for (uint64 i = 0; i < buffer_slots; ++i) {
    data[i] = 0;
  }
}

SolutionBuffer::SolutionBuffer(const std::string& filename) {
  bool success = Read(filename);
  if (!success) {
    throw "Failed to read SolutionBuffer from file.";
  }
}

SolutionBuffer::~SolutionBuffer() {
  delete data;
}

Solution SolutionBuffer::Get(uint64 index) {
  const uint64 slot_index = index / 32;
  const uint64 slot = data[slot_index];
  const int index_within_slot = index % 32;
  const int bit_offset = 2 * index_within_slot;
  const Solution value = (Solution)((slot >> bit_offset) & 3);
  return value;
}

void SolutionBuffer::Set(uint64 index, Solution value) {
  const uint64 slot_index = index / 32;
  const uint64 slot = data[slot_index];
  const int index_within_slot = index % 32;
  const int bit_offset = 2 * index_within_slot;
  const uint64 cleared_slot = slot & ~(3 << bit_offset);
  const uint64 shifted_value = ((uint64)((int)value)) << bit_offset;
  data[slot_index] = shifted_value | cleared_slot;
}

uint64 SolutionBuffer::Length() {
  return length;
}

bool SolutionBuffer::Write(const std::string& filename) {
  std::ofstream file(filename, std::ios::out | std::ios::binary);
  const uint64 version = 1;
  file.write((char*)&version, sizeof version);
  file.write((char*)&length, sizeof length);
  uint64 buffer_slots = length / 32;
  if (length % 32 != 0) {
    buffer_slots += 1;
  }
  for (uint64 i = 0; i < buffer_slots; ++i) {
    file.write((char*)&data[i], sizeof data[i]);
  }
  file.close();
  return true;
}

bool SolutionBuffer::Read(const std::string& filename) {
  std::ifstream file(filename, std::ios::in | std::ios::binary);
  uint64 version;
  file.read((char*)&version, sizeof version);
  file.read((char*)&length, sizeof length);
  uint64 buffer_slots = length / 32;
  if (length % 32 != 0) {
    buffer_slots += 1;
  }
  data = new uint64[buffer_slots];
  for (uint64 i = 0; i < buffer_slots; ++i) {
    file.read((char*)&data[i], sizeof data[i]);
  }
  file.close();
  return true;
}