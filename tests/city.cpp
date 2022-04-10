#include <cstddef>

#include <gtest/gtest.h>

#include "city.h"
#include "exceptions.h"
#include "person.h"

TEST(City, exceptions) {
#define C(dt, size, rtime) City(0, dt, size, rtime)

  EXPECT_NO_THROW(C(1, 1, 1));

  EXPECT_THROW(C(-1, 1, 1), RequiredPositiveDoubleValueException);
  EXPECT_THROW(C(1, -1, 1), RequiredPositiveDoubleValueException);
  EXPECT_THROW(C(1, 1, -1), RequiredPositiveDoubleValueException);

  EXPECT_THROW(C(0, 1, 1), RequiredPositiveDoubleValueException);
  EXPECT_THROW(C(1, 0, 1), RequiredPositiveDoubleValueException);
  EXPECT_THROW(C(1, 1, 0), RequiredPositiveDoubleValueException);

#undef C
}

TEST(City, add_person) {
#define P(x, y)                                                                \
  Person { x, y, 0, 0, 1, Person::InfectionStatus::GREEN }
#define SIZE 100.0
#define EPS 0.1

  City c = {0, 1, SIZE, 1};

  // middle
  EXPECT_NO_THROW(c.add_person(P(SIZE / 2, SIZE / 2)));

  // sides
  EXPECT_NO_THROW(c.add_person(P(SIZE / 2, 0.0)));
  EXPECT_NO_THROW(c.add_person(P(SIZE, SIZE / 2)));
  EXPECT_NO_THROW(c.add_person(P(SIZE / 2, SIZE)));
  EXPECT_NO_THROW(c.add_person(P(0.0, SIZE / 2)));

  // corners
  EXPECT_NO_THROW(c.add_person(P(0.0, 0.0)));
  EXPECT_NO_THROW(c.add_person(P(SIZE, 0.0)));
  EXPECT_NO_THROW(c.add_person(P(SIZE, SIZE)));
  EXPECT_NO_THROW(c.add_person(P(0.0, SIZE)));

  // outside (bottom left corner going counter-clockwise)
  EXPECT_THROW(c.add_person(P(-EPS, -EPS)), OutOfCityBoundsException);
  EXPECT_THROW(c.add_person(P(SIZE / 2, -EPS)), OutOfCityBoundsException);
  EXPECT_THROW(c.add_person(P(SIZE + EPS, -EPS)), OutOfCityBoundsException);
  EXPECT_THROW(c.add_person(P(SIZE + EPS, SIZE / 2)), OutOfCityBoundsException);
  EXPECT_THROW(c.add_person(P(SIZE + EPS, SIZE + EPS)),
               OutOfCityBoundsException);
  EXPECT_THROW(c.add_person(P(SIZE / 2, SIZE + EPS)), OutOfCityBoundsException);
  EXPECT_THROW(c.add_person(P(-EPS, SIZE + EPS)), OutOfCityBoundsException);
  EXPECT_THROW(c.add_person(P(-EPS, SIZE / 2)), OutOfCityBoundsException);

#undef P
#undef SIZE
#undef EPS
}

TEST(City, is_in_bound) {
#define P(x, y)                                                                \
  Person { x, y, 0, 0, 1, Person::InfectionStatus::GREEN }
#define SIZE 100.0
#define EPS 0.1

  City c = {0, 1, SIZE, 1};

  // middle
  EXPECT_TRUE(c.is_in_bound(P(SIZE / 2, SIZE / 2)));

  // sides
  EXPECT_TRUE(c.is_in_bound(P(SIZE / 2, 0.0)));
  EXPECT_TRUE(c.is_in_bound(P(SIZE, SIZE / 2)));
  EXPECT_TRUE(c.is_in_bound(P(SIZE / 2, SIZE)));
  EXPECT_TRUE(c.is_in_bound(P(0.0, SIZE / 2)));

  // corners
  EXPECT_TRUE(c.is_in_bound(P(0.0, 0.0)));
  EXPECT_TRUE(c.is_in_bound(P(SIZE, 0.0)));
  EXPECT_TRUE(c.is_in_bound(P(SIZE, SIZE)));
  EXPECT_TRUE(c.is_in_bound(P(0.0, SIZE)));

  // outside (bottom left corner going counter-clockwise)
  EXPECT_FALSE(c.is_in_bound(P(-EPS, -EPS)));
  EXPECT_FALSE(c.is_in_bound(P(SIZE / 2, -EPS)));
  EXPECT_FALSE(c.is_in_bound(P(SIZE + EPS, -EPS)));
  EXPECT_FALSE(c.is_in_bound(P(SIZE + EPS, SIZE / 2)));
  EXPECT_FALSE(c.is_in_bound(P(SIZE + EPS, SIZE + EPS)));
  EXPECT_FALSE(c.is_in_bound(P(SIZE / 2, SIZE + EPS)));
  EXPECT_FALSE(c.is_in_bound(P(-EPS, SIZE + EPS)));
  EXPECT_FALSE(c.is_in_bound(P(-EPS, SIZE / 2)));

#undef P
#undef SIZE
#undef EPS
}

TEST(City, people) {
#define P(r) Person(1, 1, 0, 0, r, Person::InfectionStatus::GREEN)
  City c = {0, 1, 100, 1};

  c.add_person(P(1));
  c.add_person(P(2));
  c.add_person(P(3));
  c.add_person(P(4));
  c.add_person(P(5));

  const auto& people = c.people();

  EXPECT_EQ(people.size(), 5);

  for (std::size_t i = 0; i < people.size(); i++) {
    const auto r = people[i].radius();

    EXPECT_EQ(r, i + 1);
  }

#undef P
}
