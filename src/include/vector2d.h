#ifndef VECTOR_2D_H
#define VECTOR_2D_H

#include <iostream>
#include <tuple>

class Vector2d {
public:
  Vector2d() noexcept;
  Vector2d(const double, const double) noexcept;
  explicit Vector2d(const std::tuple<double, double>) noexcept;

  Vector2d(const Vector2d&) = default;
  Vector2d& operator=(const Vector2d&) = default;
  Vector2d(Vector2d&&) = default;
  Vector2d& operator=(Vector2d&&) = default;
  ~Vector2d() = default;

  double x() const noexcept;
  void x(const double) noexcept;
  double y() const noexcept;
  void y(const double) noexcept;
  std::tuple<double, double> tuple() const noexcept;
  void tuple(const std::tuple<double, double>) noexcept;
  double length() const noexcept;
  void reflect(const Vector2d) noexcept;
  static double distance(const Vector2d&, const Vector2d&) noexcept;

  bool operator==(const Vector2d&) const noexcept;
  bool operator!=(const Vector2d&) const noexcept;
  Vector2d operator+() const noexcept;
  Vector2d operator-() const noexcept;
  Vector2d operator+(const Vector2d&) const noexcept;
  Vector2d operator-(const Vector2d&) const noexcept;
  Vector2d operator*(const double) const noexcept;  // scalar multiplication
  double operator*(const Vector2d&) const noexcept; // dot product
  Vector2d& operator+=(const Vector2d&) noexcept;
  Vector2d& operator-=(const Vector2d&) noexcept;
  Vector2d& operator*=(const double) noexcept; // scalar multiplication
private:
  double m_x, m_y;
};

Vector2d operator*(const double, const Vector2d&) noexcept;
std::ostream& operator<<(std::ostream&, const Vector2d&) noexcept;

#endif // VECTOR_2D_H
