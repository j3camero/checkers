#include "piece-count.h"

#include <fstream>

PieceCount::PieceCount(int nbk, int nwk, int nbp, int nwp)
  : nbk(nbk), nwk(nwk), nbp(nbp), nwp(nwp) {
  // Body is empty because of initializer lists.
}

PieceCount::PieceCount(const PieceCount& p) {
  nbk = p.nbk;
  nwk = p.nwk;
  nbp = p.nbp;
  nwp = p.nwp;
}

PieceCount PieceCount::Mirror() {
  // Black and white are reversed.
  return PieceCount(nwk, nbk, nwp, nbp);
}

bool PieceCount::operator==(const PieceCount& other) const {
  return (nbk == other.nbk && nwk == other.nwk &&
          nbp == other.nbp && nwp == other.nwp);
}

bool PieceCount::operator!=(const PieceCount& other) const {
  return !(*this == other);
}

bool PieceCount::operator<(const PieceCount& other) const {
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

std::ostream& operator<<(std::ostream &out, const PieceCount& p) {
  out << p.nwk << p.nbk << p.nbp << p.nwp;
  return out;
}
