#include <tuple>

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
    : m_A{direction.y()}, m_B{-direction.x()}, m_C{-direction.y() * base.x() +
                                                   direction.x() * base.y()}

{}

/**
 * # Exceptions
 * - Throws Line2dException when both first and second numers of @data are equal
 * to 0
 */
Line2d::Line2d(const std::tuple<double, double, double> data)
    : m_A{std::get<0>(data)}, m_B{std::get<1>(data)}, m_C{std::get<2>(data)} {}