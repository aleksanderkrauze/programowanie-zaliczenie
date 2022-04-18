#include <cstdlib>

#include "utils.h"

namespace utils {

bool double_equality(const double lhs, const double rhs) {
#ifndef EPSILON
  const double eps = 1e-9;
#else
  const double eps = EPSILON;
#endif

  if (std::abs(lhs - rhs) <= eps) {
    return true;
  }

  return false;
}

} // namespace utils