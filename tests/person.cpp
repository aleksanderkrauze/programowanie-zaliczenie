#include <utility>

#include <gtest/gtest.h>

#include "exceptions.h"
#include "person.h"

TEST(Person, infection_status_to_colour) {
  EXPECT_STREQ(
    Person::infection_status_to_colour(Person::InfectionStatus::GREEN).c_str(),
    "green");
  EXPECT_STREQ(
    Person::infection_status_to_colour(Person::InfectionStatus::RED).c_str(),
    "red");
  EXPECT_STREQ(
    Person::infection_status_to_colour(Person::InfectionStatus::BLUE).c_str(),
    "blue");
}

/* **********************************************
 * Person's getters and setters tests
 * *********************************************/

TEST(Person, getters) {
  const Person p{1.0, 2.0, 3.0, -2.0, 5.0, Person::InfectionStatus::GREEN};

  EXPECT_EQ(p.x(), 1.0);
  EXPECT_EQ(p.y(), 2.0);
  EXPECT_EQ(p.vx(), 3.0);
  EXPECT_EQ(p.vy(), -2.0);
  EXPECT_EQ(p.radius(), 5.0);
  EXPECT_EQ(p.time_of_infection(), 0.0);
  EXPECT_EQ(p.infection_status(), Person::InfectionStatus::GREEN);
}

TEST(Person, setters) {
  Person p{0, 0, 0, 0, 1, Person::InfectionStatus::GREEN};

  p.x(3.0);
  EXPECT_EQ(p.x(), 3.0);
  p.y(2.0);
  EXPECT_EQ(p.y(), 2.0);
  p.vx(7.0);
  EXPECT_EQ(p.vx(), 7.0);
  p.vy(-3.0);
  EXPECT_EQ(p.vy(), -3.0);
  p.time_of_infection(120.0);
  EXPECT_EQ(p.time_of_infection(), 120.0);
  p.infection_status(Person::InfectionStatus::RED);
  EXPECT_EQ(p.infection_status(), Person::InfectionStatus::RED);
}

/* **********************************************
 * Person::Person exceptions tests
 * *********************************************/

TEST(Person, exceptions) {
#define P(r) Person(0, 0, 0, 0, r, Person::InfectionStatus::GREEN)

  EXPECT_NO_THROW(P(10.0));

  EXPECT_THROW(P(0), RequiredPositiveDoubleValueException);
  EXPECT_THROW(P(-0.001), RequiredPositiveDoubleValueException);
  EXPECT_THROW(P(-0.5), RequiredPositiveDoubleValueException);
  EXPECT_THROW(P(-5), RequiredPositiveDoubleValueException);

#undef P
}

/* **********************************************
 * Person::move() tests
 * *********************************************/
void test_move(double x, double y, double vx, double vy, double xdt, double ydt,
               double dt, double city_size) {
  Person p{x, y, vx, vy, 1.0, Person::InfectionStatus::GREEN};
  p.move(dt, city_size);

  EXPECT_EQ(std::make_pair(p.x(), p.y()), std::make_pair(xdt, ydt))
    << "Test failed with following values: "
    << "x=" << x << ", y=" << y << ", vx=" << vx << ", vy=" << vy
    << ", xdt=" << xdt << ", ydt=" << ydt << ", city_size=" << city_size;
}

TEST(Person_move, exceptions) {
  Person p{0, 0, 0, 0, 1, Person::InfectionStatus::GREEN};

  // No Exceptions of correct arguments
  EXPECT_NO_THROW(p.move(1, 100));

  // Throw RequiredPositiveDoubleValueException on negative or 0 arguments
  EXPECT_THROW(p.move(0, 1), RequiredPositiveDoubleValueException);
  EXPECT_THROW(p.move(1, 0), RequiredPositiveDoubleValueException);
  EXPECT_THROW(p.move(-1, 1), RequiredPositiveDoubleValueException);
  EXPECT_THROW(p.move(1, -1), RequiredPositiveDoubleValueException);
}

TEST(Person_move, normal_in_the_middle) {
  test_move(5, 5, 1, 2, 6, 7, 1, 10);
  test_move(5, 5, -2, 0, 3, 5, 1, 10);
  test_move(5, 5, -1, -2, 4, 3, 1, 10);
}

TEST(Person_move, touch_perimeter_from_outside) {
  // test_move(0, 0, );
}

/* **********************************************
 * Person::is_in_infection_range() tests
 * *********************************************/
#define P(x, y, r)                                                             \
  Person { x, y, 0, 0, r, Person::InfectionStatus::GREEN }

TEST(Person_is_in_infection_range, apart) {
  EXPECT_FALSE(Person::is_in_infection_range(P(0, 0, 1), P(10, 10, 1)));
  EXPECT_FALSE(Person::is_in_infection_range(P(0, 0, 1), P(10, 10, 8)));
  EXPECT_FALSE(Person::is_in_infection_range(P(-1, -1, 1), P(1, 1, 1)));
}

TEST(Person_is_in_infection_range, tangential_outside) {
  EXPECT_TRUE(Person::is_in_infection_range(P(0, 0, 0.5), P(0, 1, 0.5)));
  EXPECT_TRUE(Person::is_in_infection_range(P(0, 0, 5), P(0, 10, 5)));
  EXPECT_TRUE(Person::is_in_infection_range(P(4, 4, 2), P(4, 1, 1)));
}

TEST(Person_is_in_infection_range, overlaping) {
  EXPECT_TRUE(Person::is_in_infection_range(P(0, 0, 1), P(1, 0, 1)));
  EXPECT_TRUE(Person::is_in_infection_range(P(0, 0, 1), P(1, 1, 2)));
  EXPECT_TRUE(Person::is_in_infection_range(P(0, 0, 2), P(1, 1, 2)));
}

TEST(Person_is_in_infection_range, inside) {
  EXPECT_TRUE(Person::is_in_infection_range(P(0, 0, 1), P(0, 0, 1)));
  EXPECT_TRUE(Person::is_in_infection_range(P(0, 0, 5), P(0, 0, 1)));
  EXPECT_TRUE(Person::is_in_infection_range(P(0, 0, 5), P(1, 1, 1)));
}

#undef P
