#ifndef _COMBINATOR_H_
#define _COMBINATOR_H_

#include <vector>

class Combinator {
 public:
  Combinator(int n, int k);
  //bool Increment(const std::vector<int> square_numbers = NULL);

private:
  int n;
  int k;
  std::vector<int> indices;
};

#endif
