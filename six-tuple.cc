#include "six-tuple.h"

SixTuple::SixTuple(int nbk, int nwk, int nbp, int nwp, int rbp, int rwp)
  : FourTuple(nbk, nwk, nbp, nwp), rbp(rbp), rwp(rwp) {
  // Body is empty because of initializer lists.
}

SixTuple::SixTuple(const SixTuple& d)
  : FourTuple(d) {
  rbp = d.rbp;
  rwp = d.rwp;
}

SixTuple SixTuple::Mirror() {
  // Black and white are reversed.
  return SixTuple(nwk, nbk, nwp, nbp, rwp, rbp);
}

bool SixTuple::operator==(const SixTuple& other) const {
  return (nbk == other.nbk && nwk == other.nwk &&
          nbp == other.nbp && nwp == other.nwp &&
          rbp == other.rbp && rwp == other.rwp);
}

bool SixTuple::operator!=(const SixTuple& other) const {
  return !(*this == other);
}

std::ostream& operator<<(std::ostream &out, const SixTuple& p) {
  out << p.nwk << p.nbk << p.nbp << p.nwp << "." << p.rbp << p.rwp;
  return out;
}
