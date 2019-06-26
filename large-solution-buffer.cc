#include "large-solution-buffer.h"
#include "medium-solution-buffer.h"

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
