#include <gtest/gtest.h>

#include <tuple>

#include "exceptions.h"
#include "line2d.h"
#include "vector2d.h"

TEST(Line2d, constructors_failing) {
  ASSERT_THROW(Line2d(0, 0, 1), Line2dException);
  ASSERT_THROW(Line2d(Vector2d(0, 0), Vector2d(0, 0)), Line2dException);
  ASSERT_THROW(Line2d(std::make_tuple(0, 0, 1)), Line2dException);
}

TEST(Line2d, constructors_successfull) {
  // Normal constructor
  ASSERT_NO_THROW(Line2d(1, -5, 7));
  ASSERT_NO_THROW(Line2d(0, 1, -7));
  ASSERT_NO_THROW(Line2d(1, 0, 17));

  // Vectors constructor
  ASSERT_NO_THROW(Line2d(Vector2d(0, 0), Vector2d(1, 0)));
  ASSERT_NO_THROW(Line2d(Vector2d(0, 0), Vector2d(1e-9, 1e-9)));

  // Tuple constructor
  ASSERT_NO_THROW(Line2d(std::make_tuple(2, 2, 4)));
  ASSERT_NO_THROW(Line2d(std::make_tuple(0, 2, 4)));
  ASSERT_NO_THROW(Line2d(std::make_tuple(2, 0, 4)));
}

TEST(Line2d, no_intersection) {
  // The same line
  const Line2d l{0, 1, 2};
  ASSERT_FALSE(Line2d::intersection(l, l));

  // Parallel
  const Line2d l0{Vector2d{0, 0}, Vector2d{0, 1}};
  const Line2d l1{Vector2d{1, 0}, Vector2d{0, 1}};
  ASSERT_FALSE(Line2d::intersection(l0, l1));
}

TEST(Line2d, normal) {
  ASSERT_EQ(Line2d({1, 0}, {0, 1}).normal(), Vector2d(1, 0));
  ASSERT_EQ(Line2d({1, 1}, {-1, 0}).normal(), Vector2d(0, 1));
  ASSERT_EQ(Line2d({0, 0}, {0, -1}).normal(), Vector2d(-1, 0));
  ASSERT_EQ(Line2d({0, 0}, {1, 0}).normal(), Vector2d(0, -1));
}

TEST(Line2d, intersection) {
  // Intersection of axis is (0, 0)
  ASSERT_EQ(Line2d::intersection(Line2d{Vector2d{}, Vector2d{0, 1}},
                                 Line2d{Vector2d{}, Vector2d{1, 0}})
              .value(),
            Vector2d());

  // Intersection of y = x and x = 1 is (1, 1)
  ASSERT_EQ(Line2d::intersection(Line2d{Vector2d{}, Vector2d{2, 2}},
                                 Line2d{Vector2d{1, 0}, Vector2d{0, 0.1}})
              .value(),
            Vector2d(1, 1));

  // Interscetion of y = x - 2 and y = -3x -2 is (0, -2)
  ASSERT_EQ(Line2d::intersection(Line2d{Vector2d{2, 0}, Vector2d{-1, -1}},
                                 Line2d{Vector2d{-2, 4}, Vector2d{1, -3}})
              .value(),
            Vector2d(0, -2));
}