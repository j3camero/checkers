// A simple benchmark for the Enumerator object.
//
// It does one complete loop of a large Enumerator, incrementing through all
// its positions. It's recommended to use the "time" program to get timing info:
//
//   time ./enumerator-benchmark
#include "enumerator.h"

int main(int argc, char **argv) {
  std::cout << "argc: " << argc << std::endl;
  for (int i = 0; i < argc; ++i) {
    std::cout << "argv[" << i << "]: " << argv[i] << std::endl;
  }
  if (argc != 2) {
    std::cout << "USAGE: time ./enumerator-benchmark 3212.66" << std::endl
              << "Any database slice of the form ABCD.EF can be specified. "
              << "For more information, see Lake (1994)." << std::endl;
    return 1;
  }
  std::string db_string(argv[1]);
  SixTuple db = SixTuple::ParseOrDie(db_string);
  Enumerator e(db);
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
