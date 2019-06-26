// A simple benchmark for the Enumerator object.
//
// It does one complete loop of a large Enumerator, incrementing through all
// its positions. It's recommended to use the "time" program to get timing info:
//
//   time ./enumerator-benchmark
#include "enumerator.h"
#include "std.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    cout << "USAGE: time ./enumerator-benchmark 3212.66" << endl
              << "Any database slice of the form ABCD.EF can be specified. "
              << "For more information, see Lake (1994)." << endl;
    return 1;
  }
  string db_string(argv[1]);
  SixTuple db = SixTuple::ParseOrDie(db_string);
  Enumerator e(db);
  const uint64 n = e.NumPositions();
  cout << "Starting with position 0:" << endl << e << endl
            << "Enumerating " << n << " positions..." << endl;
  e.Increment(n - 1);
  cout << "Last position:" << endl << e << endl;
  if (!e.Increment()) {
    cerr << "Enumerator failed to overflow." << endl;
    return 1;
  }
  cout << "Done." << endl;
  return 0;
}
