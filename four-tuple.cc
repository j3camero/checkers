#include "four-tuple.h"
#include "std.h"

FourTuple::FourTuple(int nbk, int nwk, int nbp, int nwp)
  : nbk(nbk), nwk(nwk), nbp(nbp), nwp(nwp) {
  // Body is empty because of initializer lists.
}

FourTuple::FourTuple(const FourTuple& p)
  : nbk(p.nbk), nwk(p.nwk), nbp(p.nbp), nwp(p.nwp) {
  // Body is empty because of initializer lists.
}

FourTuple::FourTuple()
  : nbk(0), nwk(0), nbp(0), nwp(0) {
  // Body is empty because of initializer lists.
}

FourTuple FourTuple::Mirror() const {
  // Black and white are reversed.
  return FourTuple(nwk, nbk, nwp, nbp);
}

bool FourTuple::operator==(const FourTuple& other) const {
  return (nbk == other.nbk && nwk == other.nwk &&
          nbp == other.nbp && nwp == other.nwp);
}

bool FourTuple::operator!=(const FourTuple& other) const {
  return !(*this == other);
}

bool FourTuple::operator<(const FourTuple& other) const {
  if (nbp < other.nbp) return true;
  if (nbp > other.nbp) return false;
  if (nwp < other.nwp) return true;
  if (nwp > other.nwp) return false;
  if (nbk < other.nbk) return true;
  if (nbk > other.nbk) return false;
  if (nwk < other.nwk) return true;
  if (nwk > other.nwk) return false;
  return false;
}

ostream& operator<<(ostream &out, const FourTuple& p) {
  out << p.nwk << p.nbk << p.nbp << p.nwp;
  return out;
}
