#include "six-tuple.h"

#include "string-util.h"

SixTuple::SixTuple(int nbk, int nwk, int nbp, int nwp, int rbp, int rwp)
  : FourTuple(nbk, nwk, nbp, nwp), rbp(rbp), rwp(rwp) {
  // Body is empty because of initializer lists.
}

SixTuple::SixTuple(const SixTuple& d)
  : FourTuple(d) {
  rbp = d.rbp;
  rwp = d.rwp;
}

SixTuple::SixTuple()
  : FourTuple(0, 0, 0, 0), rbp(0), rwp(0) {
  // Body is empty because of initializer lists.
}

SixTuple SixTuple::Mirror() const {
  // Black and white are reversed.
  return SixTuple(nwk, nbk, nwp, nbp, rwp, rbp);
}

bool SixTuple::Parse(const std::string& s) {
  if (s.size() != 7) {
    return false;
  }
  if (!StringUtil::IsDigit(s[0]) || !StringUtil::IsDigit(s[1]) ||
      !StringUtil::IsDigit(s[2]) || !StringUtil::IsDigit(s[3]) ||
      s[4] != '.' || !StringUtil::IsDigit(s[5]) || !StringUtil::IsDigit(s[6])) {
    return false;
  }
  nbk = StringUtil::DigitToInt(s[0]);
  nwk = StringUtil::DigitToInt(s[1]);
  nbp = StringUtil::DigitToInt(s[2]);
  nwp = StringUtil::DigitToInt(s[3]);
  rbp = StringUtil::DigitToInt(s[5]);
  rwp = StringUtil::DigitToInt(s[6]);
  return true;
}

SixTuple SixTuple::ParseOrDie(const std::string& s) {
  SixTuple t;
  if (!t.Parse(s)) {
    throw "Invalid SixTuple or database slice identifier.";
  }
  return t;
}

bool SixTuple::operator==(const SixTuple& other) const {
  return (nbk == other.nbk && nwk == other.nwk &&
          nbp == other.nbp && nwp == other.nwp &&
          rbp == other.rbp && rwp == other.rwp);
}

bool SixTuple::operator!=(const SixTuple& other) const {
  return !(*this == other);
}

bool SixTuple::operator<(const SixTuple& other) const {
  if (nbp < other.nbp) return true;
  if (nbp > other.nbp) return false;
  if (nwp < other.nwp) return true;
  if (nwp > other.nwp) return false;
  if (nbk < other.nbk) return true;
  if (nbk > other.nbk) return false;
  if (nwk < other.nwk) return true;
  if (nwk > other.nwk) return false;
  if (rbp < other.rbp) return true;
  if (rbp > other.rbp) return false;
  if (rwp < other.rwp) return true;
  if (rwp > other.rwp) return false;
  return false;
}

std::ostream& operator<<(std::ostream &out, const SixTuple& p) {
  out << p.nwk << p.nbk << p.nbp << p.nwp << "." << p.rbp << p.rwp;
  return out;
}
