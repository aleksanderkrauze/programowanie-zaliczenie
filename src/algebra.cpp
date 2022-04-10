#include <cmath>
#include <tuple>

#include "algebra.h"

Vector2d::Vector2d() : m_x{0.0}, m_y{0.0} {}

Vector2d::Vector2d(double x, double y) : m_x{x}, m_y{y} {}

Vector2d::Vector2d(std::tuple<double, double> coordinates)
    : m_x{std::get<0>(coordinates)}, m_y{std::get<1>(coordinates)} {}

double Vector2d::x() const { return this->m_x; }

void Vector2d::x(double x) { this->m_x = x; }

double Vector2d::y() const { return this->m_y; }

void Vector2d::y(double y) { this->m_y = y; }

std::tuple<double, double> Vector2d::tuple() const {
  return std::make_tuple(this->m_x, this->m_y);
}

void Vector2d::tuple(std::tuple<double, double> coordinates) {
  auto [x, y] = coordinates;
  this->m_x = x;
  this->m_y = y;
}

double Vector2d::length() const {
  return std::sqrt((this->m_x * this->m_x) + (this->m_y * this->m_y));
}

double Vector2d::distance(const Vector2d& lhs, const Vector2d& rhs) {
  auto diff = lhs - rhs;
  return diff.length();
}

bool Vector2d::operator==(const Vector2d& rhs) const {
  return (this->m_x == rhs.m_x) && (this->m_y == rhs.m_y);
}

bool Vector2d::operator!=(const Vector2d& rhs) const { return !(*this == rhs); }

Vector2d Vector2d::operator+() const { return *this; }

Vector2d Vector2d::operator-() const {
  return Vector2d{-this->m_x, -this->m_y};
}

Vector2d Vector2d::operator+(const Vector2d& rhs) const {
  return Vector2d{this->m_x + rhs.m_x, this->m_y + rhs.m_y};
}

Vector2d Vector2d::operator-(const Vector2d& rhs) const {
  return Vector2d{this->m_x - rhs.m_x, this->m_y - rhs.m_y};
}

Vector2d Vector2d::operator*(double scalar) const {
  return Vector2d{scalar * this->m_x, scalar * this->m_y};
}

Vector2d operator*(double scalar, const Vector2d& rhs) { return rhs * scalar; }

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

Vector2d& Vector2d::operator*=(double scalar) {
  this->m_x *= scalar;
  this->m_y *= scalar;

  return *this;
}
