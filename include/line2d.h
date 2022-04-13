#ifndef LINE_2D_H
#define LINE_2D_H

#include <tuple>

#include "vector2d.h"

class Line2d {
public:
  Line2d(const double, const double, const double);
  Line2d(const Vector2d, const Vector2d);
  explicit Line2d(const std::tuple<double, double, double>);

  Line2d(const Line2d&) = default;
  Line2d& operator=(const Line2d&) = default;
  Line2d(Line2d&&) = default;
  Line2d& operator=(Line2d&&) = default;
  ~Line2d() = default;

private:
  double m_A, m_B, m_C;
};

#endif // LINE_2D_H