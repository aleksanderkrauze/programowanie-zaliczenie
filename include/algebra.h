#ifndef ALGEBRA_H
#define ALGEBRA_H

#include <tuple>

class Vector2d {
public:
  Vector2d();
  Vector2d(double, double);
  Vector2d(std::tuple<double, double>);

  Vector2d(const Vector2d&) = default;
  Vector2d& operator=(const Vector2d&) = default;
  Vector2d(Vector2d&&) = default;
  Vector2d& operator=(Vector2d&&) = default;
  ~Vector2d() = default;

  double x() const;
  void x(double);
  double y() const;
  void y(double);
  std::tuple<double, double> tuple() const;
  void tuple(std::tuple<double, double>);

  double length() const;

  static double distance(const Vector2d&, const Vector2d&);

  bool operator==(const Vector2d&) const;
  bool operator!=(const Vector2d&) const;
  Vector2d operator+() const;
  Vector2d operator-() const;
  Vector2d operator+(const Vector2d&) const;
  Vector2d operator-(const Vector2d&) const;
  Vector2d operator*(double) const;        // scalar multiplication
  double operator*(const Vector2d&) const; // dot product
  Vector2d& operator+=(const Vector2d&);
  Vector2d& operator-=(const Vector2d&);
  Vector2d& operator*=(double); // scalar multiplication

private:
  double m_x, m_y;
};

Vector2d operator*(double, const Vector2d&);

#endif // ALGEBRA_H
