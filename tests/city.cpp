#include <cstddef>

#include <gtest/gtest.h>

#include "city.h"
#include "exceptions.h"
#include "person.h"

TEST(City, exceptions) {
  const auto C = [](const double size, const double time, const double dt,
                    const double rtime) {
    return City{size, time, dt, rtime};
  };

  ASSERT_NO_THROW(C(1, 1, 1, 1));

  ASSERT_THROW(C(-1, 1, 1, 1), RequiredPositiveDoubleValueException);
  ASSERT_THROW(C(1, -1, 1, 1), RequiredPositiveDoubleValueException);
  ASSERT_THROW(C(1, 1, -1, 1), RequiredPositiveDoubleValueException);
  ASSERT_THROW(C(1, 1, 1, -1), RequiredPositiveDoubleValueException);

  ASSERT_THROW(C(0, 1, 1, 1), RequiredPositiveDoubleValueException);
  ASSERT_THROW(C(1, 0, 1, 1), RequiredPositiveDoubleValueException);
  ASSERT_THROW(C(1, 1, 0, 1), RequiredPositiveDoubleValueException);
  ASSERT_THROW(C(1, 1, 1, 0), RequiredPositiveDoubleValueException);
}

TEST(City, add_person) {
  const auto P = [](const double x, const double y) {
    return Person{{x, y}, {0, 0}, 1};
  };
  const auto SIZE = 100.0;
  const auto EPS = 0.1;

  City c{SIZE, 1, 1, 1};

  // middle
  ASSERT_NO_THROW(c.add_person(P(SIZE / 2, SIZE / 2)));

  // sides
  ASSERT_NO_THROW(c.add_person(P(SIZE / 2, 0.0)));
  ASSERT_NO_THROW(c.add_person(P(SIZE, SIZE / 2)));
  ASSERT_NO_THROW(c.add_person(P(SIZE / 2, SIZE)));
  ASSERT_NO_THROW(c.add_person(P(0.0, SIZE / 2)));

  // corners
  ASSERT_NO_THROW(c.add_person(P(0.0, 0.0)));
  ASSERT_NO_THROW(c.add_person(P(SIZE, 0.0)));
  ASSERT_NO_THROW(c.add_person(P(SIZE, SIZE)));
  ASSERT_NO_THROW(c.add_person(P(0.0, SIZE)));

  // outside (bottom left corner going counter-clockwise)
  ASSERT_THROW(c.add_person(P(-EPS, -EPS)), OutOfCityBoundsException);
  ASSERT_THROW(c.add_person(P(SIZE / 2, -EPS)), OutOfCityBoundsException);
  ASSERT_THROW(c.add_person(P(SIZE + EPS, -EPS)), OutOfCityBoundsException);
  ASSERT_THROW(c.add_person(P(SIZE + EPS, SIZE / 2)), OutOfCityBoundsException);
  ASSERT_THROW(c.add_person(P(SIZE + EPS, SIZE + EPS)),
               OutOfCityBoundsException);
  ASSERT_THROW(c.add_person(P(SIZE / 2, SIZE + EPS)), OutOfCityBoundsException);
  ASSERT_THROW(c.add_person(P(-EPS, SIZE + EPS)), OutOfCityBoundsException);
  ASSERT_THROW(c.add_person(P(-EPS, SIZE / 2)), OutOfCityBoundsException);
}

TEST(City, is_in_bound) {
  const auto P = [](const double x, const double y) {
    return Person{{x, y}, {0, 0}, 1};
  };
  const auto SIZE = 100.0;
  const auto EPS = 0.1;

  City c{SIZE, 1, 1, 1};

  // middle
  ASSERT_TRUE(c.is_in_bound(P(SIZE / 2, SIZE / 2)));

  // sides
  ASSERT_TRUE(c.is_in_bound(P(SIZE / 2, 0.0)));
  ASSERT_TRUE(c.is_in_bound(P(SIZE, SIZE / 2)));
  ASSERT_TRUE(c.is_in_bound(P(SIZE / 2, SIZE)));
  ASSERT_TRUE(c.is_in_bound(P(0.0, SIZE / 2)));

  // corners
  ASSERT_TRUE(c.is_in_bound(P(0.0, 0.0)));
  ASSERT_TRUE(c.is_in_bound(P(SIZE, 0.0)));
  ASSERT_TRUE(c.is_in_bound(P(SIZE, SIZE)));
  ASSERT_TRUE(c.is_in_bound(P(0.0, SIZE)));

  // outside (bottom left corner going counter-clockwise)
  ASSERT_FALSE(c.is_in_bound(P(-EPS, -EPS)));
  ASSERT_FALSE(c.is_in_bound(P(SIZE / 2, -EPS)));
  ASSERT_FALSE(c.is_in_bound(P(SIZE + EPS, -EPS)));
  ASSERT_FALSE(c.is_in_bound(P(SIZE + EPS, SIZE / 2)));
  ASSERT_FALSE(c.is_in_bound(P(SIZE + EPS, SIZE + EPS)));
  ASSERT_FALSE(c.is_in_bound(P(SIZE / 2, SIZE + EPS)));
  ASSERT_FALSE(c.is_in_bound(P(-EPS, SIZE + EPS)));
  ASSERT_FALSE(c.is_in_bound(P(-EPS, SIZE / 2)));
}

TEST(City, people) {
  const auto P = [](const double r) { return Person{{1, 1}, {0, 0}, r}; };
  City c = {100, 1, 1, 1};

  c.add_person(P(1));
  c.add_person(P(2));
  c.add_person(P(3));
  c.add_person(P(4));
  c.add_person(P(5));

  const auto& people = c.people();

  ASSERT_EQ(people.size(), 5);

  for (std::size_t i = 0; i < people.size(); i++) {
    const auto r = people[i].radius();

    ASSERT_EQ(r, i + 1);
  }
}
