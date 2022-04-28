#include <cassert>
#include <cmath>
#include <ostream>
#include <tuple>

#include "utils.h"
#include "vector2d.h"

Vector2d::Vector2d() noexcept : m_x{0.0}, m_y{0.0} {}

Vector2d::Vector2d(const double x, const double y) noexcept : m_x{x}, m_y{y} {}

Vector2d::Vector2d(const std::tuple<double, double> coordinates) noexcept
    : m_x{std::get<0>(coordinates)}, m_y{std::get<1>(coordinates)} {}

double Vector2d::x() const noexcept { return this->m_x; }

void Vector2d::x(const double x) noexcept { this->m_x = x; }

double Vector2d::y() const noexcept { return this->m_y; }

void Vector2d::y(const double y) noexcept { this->m_y = y; }

std::tuple<double, double> Vector2d::tuple() const noexcept {
  return std::make_tuple(this->m_x, this->m_y);
}

void Vector2d::tuple(const std::tuple<double, double> coordinates) noexcept {
  const auto [x, y] = coordinates;
  this->m_x = x;
  this->m_y = y;
}

double Vector2d::length() const noexcept {
  const auto square = [](auto x) -> double { return x * x; };
  return std::sqrt(square(this->m_x) + square(this->m_y));
}

void Vector2d::reflect(const Vector2d normal) noexcept {
  assert(normal.length() == 1.0);

  *this = *this - 2 * (*this * normal) * normal;
}

void Vector2d::normalize() noexcept {
  const auto length = this->length();

  *this *= 1.0 / length;
}

double Vector2d::distance(const Vector2d& lhs, const Vector2d& rhs) noexcept {
  const auto diff = lhs - rhs;
  return diff.length();
}

bool Vector2d::operator==(const Vector2d& rhs) const noexcept {
  const auto distance = Vector2d::distance(*this, rhs);

  return utils::double_equality(distance, 0.0);
}

bool Vector2d::operator!=(const Vector2d& rhs) const noexcept {
  return !(*this == rhs);
}

Vector2d Vector2d::operator+() const noexcept { return *this; }

Vector2d Vector2d::operator-() const noexcept {
  const Vector2d zero{};
  return zero - *this;
}

Vector2d Vector2d::operator+(const Vector2d& rhs) const noexcept {
  Vector2d v{*this};
  v += rhs;
  return v;
}

Vector2d Vector2d::operator-(const Vector2d& rhs) const noexcept {
  Vector2d v{*this};
  v -= rhs;
  return v;
}

Vector2d Vector2d::operator*(const double scalar) const noexcept {
  Vector2d v{*this};
  v *= scalar;
  return v;
}

Vector2d operator*(const double scalar, const Vector2d& rhs) noexcept {
  return rhs * scalar;
}

double Vector2d::operator*(const Vector2d& rhs) const noexcept {
  return (this->m_x * rhs.m_x) + (this->m_y * rhs.m_y);
}

Vector2d& Vector2d::operator+=(const Vector2d& rhs) noexcept {
  this->m_x += rhs.m_x;
  this->m_y += rhs.m_y;

  return *this;
}

Vector2d& Vector2d::operator-=(const Vector2d& rhs) noexcept {
  this->m_x -= rhs.m_x;
  this->m_y -= rhs.m_y;

  return *this;
}

Vector2d& Vector2d::operator*=(const double scalar) noexcept {
  this->m_x *= scalar;
  this->m_y *= scalar;

  return *this;
}

std::ostream& operator<<(std::ostream& os, const Vector2d& v) noexcept {
  const auto [x, y] = v.tuple();
  os << "(" << x << ", " << y << ")";

  return os;
}
