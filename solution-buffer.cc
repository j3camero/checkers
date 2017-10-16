#include "solution-buffer.h"

#include <cassert>

SolutionBuffer::SolutionBuffer(uint64 _length) {
  length = _length;
  uint64 buffer_slots = length / 32;
  if (length % 32 != 0) {
    buffer_slots += 1;
  }
  data = new uint64[buffer_slots];
}

SolutionBuffer::SolutionBuffer(const std::string& filename) {
  bool success = Read(filename);
  assert(success);
}

SolutionBuffer::~SolutionBuffer() {
  delete data;
}

Solution SolutionBuffer::Get(uint64 index) {
  return Unknown;
}

void SolutionBuffer::Set(uint64 index, Solution value) {
  
}

uint64 SolutionBuffer::Length() {
  return length;
}

bool SolutionBuffer::Write(const std::string& filename) {
  return false;
}

bool SolutionBuffer::Read(const std::string& filename) {
  return false;
}
