#include <cmath>
#include <tuple>

#include "vector2d.h"

Vector2d::Vector2d() : m_x{0.0}, m_y{0.0} {}

Vector2d::Vector2d(const double x, const double y) : m_x{x}, m_y{y} {}

Vector2d::Vector2d(const std::tuple<double, double> coordinates)
    : m_x{std::get<0>(coordinates)}, m_y{std::get<1>(coordinates)} {}

double Vector2d::x() const { return this->m_x; }

void Vector2d::x(const double x) { this->m_x = x; }

double Vector2d::y() const { return this->m_y; }

void Vector2d::y(const double y) { this->m_y = y; }

std::tuple<double, double> Vector2d::tuple() const {
  return std::make_tuple(this->m_x, this->m_y);
}

void Vector2d::tuple(const std::tuple<double, double> coordinates) {
  const auto [x, y] = coordinates;
  this->m_x = x;
  this->m_y = y;
}

double Vector2d::length() const {
  const auto square = [](auto x) { return x * x; };
  return std::sqrt(square(this->m_x) + square(this->m_y));
}

double Vector2d::distance(const Vector2d& lhs, const Vector2d& rhs) {
  const auto diff = lhs - rhs;
  return diff.length();
}

bool Vector2d::operator==(const Vector2d& rhs) const {
  const auto epsilon = 1e-9;
  const auto distance = Vector2d::distance(*this, rhs);

  return distance < epsilon;
}

bool Vector2d::operator!=(const Vector2d& rhs) const { return !(*this == rhs); }

Vector2d Vector2d::operator+() const { return *this; }

Vector2d Vector2d::operator-() const {
  const Vector2d zero{};
  return zero - *this;
}

Vector2d Vector2d::operator+(const Vector2d& rhs) const {
  Vector2d v{*this};
  v += rhs;
  return v;
}

Vector2d Vector2d::operator-(const Vector2d& rhs) const {
  Vector2d v{*this};
  v -= rhs;
  return v;
}

Vector2d Vector2d::operator*(const double scalar) const {
  Vector2d v{*this};
  v *= scalar;
  return v;
}

Vector2d operator*(const double scalar, const Vector2d& rhs) { return rhs * scalar; }

double Vector2d::operator*(const Vector2d& rhs) const {
  return (this->m_x * rhs.m_x) + (this->m_y * rhs.m_y);
}

Vector2d& Vector2d::operator+=(const Vector2d& rhs) {
  this->m_x += rhs.m_x;
  this->m_y += rhs.m_y;

  return *this;
}

Vector2d& Vector2d::operator-=(const Vector2d& rhs) {
  this->m_x -= rhs.m_x;
  this->m_y -= rhs.m_y;

  return *this;
}

Vector2d& Vector2d::operator*=(const double scalar) {
  this->m_x *= scalar;
  this->m_y *= scalar;

  return *this;
}
