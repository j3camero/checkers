#include "small-solution-buffer.h"

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
