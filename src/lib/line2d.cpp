#include <optional>
#include <tuple>

#include "constants.h"
#include "exceptions.h"
#include "line2d.h"
#include "vector2d.h"

/**
 * # Exceptions
 * - Throws Line2dException when both @A and @B are equal to 0
 */
Line2d::Line2d(const double A, const double B, const double C)
    : m_A{A}, m_B{B}, m_C{C} {
  if (A == 0 && B == 0) {
    throw Line2dException{};
  }
}

/**
 * # Exceptions
 * - Throws Line2dException when direction vector has length 0
 */
Line2d::Line2d(const Vector2d base, const Vector2d direction)
    : Line2d{direction.y(), -direction.x(),
             (-direction.y() * base.x() + direction.x() * base.y())} {}

Vector2d Line2d::normal() const noexcept {
  Vector2d v{this->m_A, this->m_B};
  v.normalize();
  
  return v;
}

/**
 * # Exceptions
 * - Throws Line2dException when both first and second numers of @data are equal
 * to 0
 */
Line2d::Line2d(const std::tuple<double, double, double> data)
    : Line2d{std::get<0>(data), std::get<1>(data), std::get<2>(data)} {}

std::optional<Vector2d> Line2d::intersection(const Line2d& lhs,
                                             const Line2d& rhs) noexcept {
  const auto A1 = lhs.m_A, B1 = lhs.m_B, C1 = lhs.m_C;
  const auto A2 = rhs.m_A, B2 = rhs.m_B, C2 = rhs.m_C;

  const auto D = A1 * B2 - A2 * B1;

  if (std::abs(D) < constants::EPSILON) {
    return std::nullopt;
  }

  const auto x = (B1 * C2 - B2 * C1) / D;
  const auto y = -(A1 * C2 - A2 * C1) / D;

  return Vector2d{x, y};
}