#include <cmath>
#include <tuple>

#include <gtest/gtest.h>

#include "vector2d.h"

TEST(Vector2d, Vector2d_default_constructor) {
  const Vector2d v{};

  ASSERT_EQ(v.x(), 0.0);
  ASSERT_EQ(v.y(), 0.0);
}

TEST(Vector2d, Vector2d_2_doubles_constructor) {
  const Vector2d v{1.0, -2.0};

  ASSERT_EQ(v.x(), 1.0);
  ASSERT_EQ(v.y(), -2.0);
}

TEST(Vector2d, Vector2d_tuple_constructor) {
  const Vector2d v{std::make_tuple(3.0, -1.0)};

  ASSERT_EQ(v.x(), 3.0);
  ASSERT_EQ(v.y(), -1.0);
}

TEST(Vector2d, x_get) {
  const Vector2d v{1.0, 0.0};

  ASSERT_EQ(v.x(), 1.0);
}

TEST(Vector2d, x_set) {
  Vector2d v{};
  v.x(1.0);

  ASSERT_EQ(v.x(), 1.0);
}

TEST(Vector2d, y_get) {
  const Vector2d v{0.0, 1.0};

  ASSERT_EQ(v.y(), 1.0);
}

TEST(Vector2d, y_set) {
  Vector2d v{};
  v.y(1.0);

  ASSERT_EQ(v.y(), 1.0);
}

TEST(Vector2d, tuple_get) {
  const Vector2d v{2.0, -3.0};

  ASSERT_EQ(v.tuple(), std::make_tuple(2.0, -3.0));
}

TEST(Vector2d, tuple_set) {
  Vector2d v{};
  v.tuple(std::make_tuple(2.0, -3.0));

  ASSERT_EQ(v.x(), 2.0);
  ASSERT_EQ(v.y(), -3.0);
}

TEST(Vector2d, length) {
  const Vector2d v1{0.0, 0.0};
  const Vector2d v2{3.0, 4.0};
  const Vector2d v3{-5.0, 12.0};
  const Vector2d v4{-1.0, -1.0};

  ASSERT_EQ(v1.length(), 0.0);
  ASSERT_EQ(v2.length(), 5.0);
  ASSERT_EQ(v3.length(), 13.0);
  ASSERT_EQ(v4.length(), std::sqrt(2.0));
}

TEST(Vector2d, reflect) {
  const Vector2d n_right{1.0, 0.0};
  const Vector2d n_upper{0.0, 1.0};
  const Vector2d n_left{-1.0, 0.0};
  const Vector2d n_bottom{0.0, -1.0};
  Vector2d v;

  // Reflection from rigth wall
  v = {1.0, 0.0};
  v.reflect(n_right);
  ASSERT_EQ(v, Vector2d(-1.0, 0.0));

  v = {1.0, 1.0};
  v.reflect(n_right);
  ASSERT_EQ(v, Vector2d(-1.0, 1.0));

  v = {2.0, -1.0};
  v.reflect(n_right);
  ASSERT_EQ(v, Vector2d(-2.0, -1.0));

  // Reflection from upper wall
  v = {0.0, 2.0};
  v.reflect(n_upper);
  ASSERT_EQ(v, Vector2d(0.0, -2.0));

  v = {0.5, 0.3};
  v.reflect(n_upper);
  ASSERT_EQ(v, Vector2d(0.5, -0.3));

  v = {-2.0, 5.0};
  v.reflect(n_upper);
  ASSERT_EQ(v, Vector2d(-2.0, -5.0));

  // Reflection from left wall
  v = {-1.0, 0.0};
  v.reflect(n_left);
  ASSERT_EQ(v, Vector2d(1.0, 0.0));

  v = {-2.0, -0.1};
  v.reflect(n_left);
  ASSERT_EQ(v, Vector2d(2.0, -0.1));

  v = {-1.0, 3.0};
  v.reflect(n_left);
  ASSERT_EQ(v, Vector2d(1.0, 3.0));

  // Reflection from bottom wall
  v = {0.0, -5.0};
  v.reflect(n_bottom);
  ASSERT_EQ(v, Vector2d(0.0, 5.0));

  v = {1.0, -3.0};
  v.reflect(n_bottom);
  ASSERT_EQ(v, Vector2d(1.0, 3.0));

  v = {-1.5, -2.0};
  v.reflect(n_bottom);
  ASSERT_EQ(v, Vector2d(-1.5, 2.0));
}

TEST(Vector2d, normalize) {
  Vector2d v;

  v = {2.0, 0.0};
  v.normalize();
  ASSERT_EQ(v, Vector2d(1.0, 0.0));

  v = {1.0, 1.0};
  v.normalize();
  ASSERT_EQ(v, Vector2d(1.0 / std::sqrt(2.0), 1.0 / std::sqrt(2.0)));

  v = {2.0, -2.0};
  v.normalize();
  ASSERT_EQ(v, Vector2d(1.0 / std::sqrt(2.0), -1.0 / std::sqrt(2.0)));

  v = {-3.0, -1.5};
  v.normalize();
  ASSERT_EQ(v, Vector2d(-2.0 / std::sqrt(5.0), -1.0 / std::sqrt(5.0)));
}

TEST(Vector2d, distance) {
  const Vector2d v0{};
  const Vector2d v1{1.0, 0.0};
  const Vector2d v2{10.0, 0.0};
  const Vector2d v3{-1.0, 0.0};
  const Vector2d v4{1.0, 1.0};
  const Vector2d v5{4.0, 5.0};

  ASSERT_EQ(Vector2d::distance(v1, v0), 1.0);
  ASSERT_EQ(Vector2d::distance(v1, v1), 0.0);
  ASSERT_EQ(Vector2d::distance(v1, v2), 9.0);
  ASSERT_EQ(Vector2d::distance(v1, v3), 2.0);
  ASSERT_EQ(Vector2d::distance(v4, v5), 5.0);
  ASSERT_EQ(Vector2d::distance(v3, v5), 5.0 * std::sqrt(2.0));
}

TEST(Vector2d, operator_eq) {
  ASSERT_EQ(Vector2d(), Vector2d());
  ASSERT_EQ(Vector2d(1.1, 2.2), Vector2d(1.1, 2.2));
  ASSERT_EQ(Vector2d(1.0, -3.0), Vector2d(1.0, -3.0));

  // precision
  ASSERT_EQ(3 * Vector2d(0.3, 0.7), Vector2d(0.9, 2.1));
}

TEST(Vector2d, operator_neq) {
  ASSERT_NE(Vector2d(1.0, 1.0), Vector2d(1.0, -1.0));
  ASSERT_NE(Vector2d(0.0, 1.0), Vector2d(0.0, 1.1));
  ASSERT_NE(Vector2d(3.0, 4.0), Vector2d(-4.0, 7.0));

  // precision
  ASSERT_NE(Vector2d(1.0, 1.0), Vector2d(1.0 + 1e-9, 1.0 - 1e-9));
}

TEST(Vector2d, operator_unary_plus) {
  ASSERT_EQ(Vector2d(), +Vector2d());
  ASSERT_EQ(Vector2d(1.0, 2.0), +Vector2d(1.0, 2.0));
  ASSERT_EQ(Vector2d(1.0, -3.0), +Vector2d(1.0, -3.0));
}

TEST(Vector2d, operator_unary_minus) {
  ASSERT_EQ(Vector2d(), -Vector2d());
  ASSERT_EQ(Vector2d(1.0, 2.0), -Vector2d(-1.0, -2.0));
  ASSERT_EQ(Vector2d(1.0, -3.0), -Vector2d(-1.0, 3.0));
  ASSERT_EQ(Vector2d(-1.0, 4.0), -Vector2d(1.0, -4.0));
  ASSERT_EQ(Vector2d(-1.0, -5.0), -Vector2d(1.0, 5.0));
}

TEST(Vector2d, operator_plus) {
  ASSERT_EQ(Vector2d() + Vector2d(), Vector2d());
  ASSERT_EQ(Vector2d(1.0, 1.0) + Vector2d(2.0, 2.0), Vector2d(3.0, 3.0));
  ASSERT_EQ(Vector2d(1.0, -2.0) + Vector2d(-5.0, 7.0), Vector2d(-4.0, 5.0));
}

TEST(Vector2d, operator_minus) {
  ASSERT_EQ(Vector2d() - Vector2d(), Vector2d());
  ASSERT_EQ(Vector2d(1.0, -1.0) - Vector2d(1.0, -1.0), Vector2d());
  ASSERT_EQ(Vector2d(3.0, -4.0) - Vector2d(5.0, 7.0), Vector2d(-2.0, -11.0));
}

TEST(Vector2d, operator_scalar_multiplication) {
  const Vector2d v0{};
  const Vector2d v1{2.03, 0.45};
  const Vector2d v2{1.0, 1.0};
  const Vector2d v3{2.0, -3.0};

  ASSERT_EQ(v0 * 100, v0);
  ASSERT_EQ(1.1101 * v0, v0);

  ASSERT_EQ(v1 * 0, v0);
  ASSERT_EQ(0 * v1, v0);

  ASSERT_EQ(v2 * 2, Vector2d(2.0, 2.0));
  ASSERT_EQ(2 * v2, Vector2d(2.0, 2.0));

  ASSERT_EQ(v3 * -0.5, Vector2d(-1.0, 1.5));
  ASSERT_EQ(-0.5 * v3, Vector2d(-1.0, 1.5));
}

TEST(Vector2d, operator_dot_product) {
  // zero sized vector
  ASSERT_EQ(Vector2d() * Vector2d(), 0.0);
  ASSERT_EQ(Vector2d() * Vector2d(1.0, -2.0), 0.0);

  // parallel
  ASSERT_EQ(Vector2d(1.0, 0.0) * Vector2d(2.0, 0.0), 2.0);
  ASSERT_EQ(Vector2d(1.0, 2.0) * Vector2d(3.0, 6.0), 15.0);
  ASSERT_EQ(Vector2d(3.0, 4.0) * Vector2d(-6.0, -8.0), -50.0);

  // 60 deg
  ASSERT_EQ(Vector2d(1.0, 0) * Vector2d(0.5, std::sqrt(3) / 2), 0.5);

  // perpendicular
  ASSERT_EQ(Vector2d(1.0, 0.0) * Vector2d(0.0, 1.0), 0.0);
  ASSERT_EQ(Vector2d(1.0, 1.0) * Vector2d(-1.0, 1.0), 0.0);
  ASSERT_EQ(Vector2d(5.0, 1.0) * Vector2d(-2.0, 10.0), 0.0);
}

TEST(Vector2d, operator_assignment_plus) {
  Vector2d v0{};
  Vector2d v1{1.0, 1.0};

  v1 += v0;
  ASSERT_EQ(v1, Vector2d(1.0, 1.0));

  v0 += v1;
  ASSERT_EQ(v0, v1);

  v1 += {2.0, -1.0};
  ASSERT_EQ(v1, Vector2d(3.0, 0.0));
}

TEST(Vector2d, operator_assignment_minus) {
  Vector2d v0{};
  Vector2d v1{1.0, 1.0};

  v1 -= v0;
  ASSERT_EQ(v1, Vector2d(1.0, 1.0));

  v0 -= v1;
  ASSERT_EQ(v0, -v1);

  v1 -= {-3.0, 4.0};
  ASSERT_EQ(v1, Vector2d(4.0, -3.0));
}

TEST(Vector2d, operator_assignment_scalar_multiplication) {
  Vector2d v0{};
  Vector2d v1{1.0, 1.0};

  v0 *= 42;
  ASSERT_EQ(v0, Vector2d(0.0, 0.0));

  v1 *= 2;
  ASSERT_EQ(v1, Vector2d(2.0, 2.0));

  v1 *= -0.25;
  ASSERT_EQ(v1, Vector2d(-0.5, -0.5));

  v1 *= 0;
  ASSERT_EQ(v1, v0);
}