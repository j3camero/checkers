#include "enumerator.h"

int main(int argc, char **argv) {
  Enumerator e(3, 2, 1, 2, 6, 6);
  const uint64 n = e.NumPositions();
  std::cout << "Starting with position 0:" << std::endl << e << std::endl
            << "Enumerating " << n << " positions..." << std::endl;
  if (e.Increment(n - 1)) {
    std::cerr << "Problem while incrementing." << std::endl;
    return 1;
  }
  std::cout << "Last position:" << std::endl << e << std::endl;
  if (!e.Increment()) {
    std::cerr << "Enumerator failed to overflow." << std::endl;
    return 1;
  }
  std::cout << "Done." << std::endl;
  return 0;
}
