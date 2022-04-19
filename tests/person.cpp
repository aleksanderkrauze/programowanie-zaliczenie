#include <utility>

#include <gtest/gtest.h>

#include "exceptions.h"
#include "person.h"
#include "vector2d.h"

TEST(Person, infection_status_to_colour) {
  ASSERT_STREQ(
    Person::infection_status_to_colour(Person::InfectionStatus::GREEN).c_str(),
    "green");
  ASSERT_STREQ(
    Person::infection_status_to_colour(Person::InfectionStatus::RED).c_str(),
    "red");
  ASSERT_STREQ(
    Person::infection_status_to_colour(Person::InfectionStatus::BLUE).c_str(),
    "blue");
}

/* **********************************************
 * Person's getters and setters tests
 * *********************************************/

TEST(Person, getters) {
  const Person p{{1.0, 2.0}, {3.0, -2.0}, 5.0, Person::InfectionStatus::GREEN};

  ASSERT_EQ(p.position(), Vector2d(1.0, 2.0));
  ASSERT_EQ(p.velocity(), Vector2d(3.0, -2.0));
  ASSERT_EQ(p.radius(), 5.0);
  ASSERT_EQ(p.time_of_infection(), 0.0);
  ASSERT_EQ(p.infection_status(), Person::InfectionStatus::GREEN);
}

TEST(Person, setters) {
  Person p{{0, 0}, {0, 0}, 1, Person::InfectionStatus::GREEN};

  p.position({3.0, 2.0});
  ASSERT_EQ(p.position(), Vector2d(3.0, 2.0));
  p.velocity({7.0, -3.0});
  ASSERT_EQ(p.velocity(), Vector2d(7.0, -3.0));
  p.time_of_infection(120.0);
  ASSERT_EQ(p.time_of_infection(), 120.0);
  p.infection_status(Person::InfectionStatus::RED);
  ASSERT_EQ(p.infection_status(), Person::InfectionStatus::RED);
}

/* **********************************************
 * Person::Person exceptions tests
 * *********************************************/

TEST(Person, exceptions) {
#define P(r) Person({0, 0}, {0, 0}, r, Person::InfectionStatus::GREEN)

  ASSERT_NO_THROW(P(10.0));

  ASSERT_THROW(P(0), RequiredPositiveDoubleValueException);
  ASSERT_THROW(P(-0.001), RequiredPositiveDoubleValueException);
  ASSERT_THROW(P(-0.5), RequiredPositiveDoubleValueException);
  ASSERT_THROW(P(-5), RequiredPositiveDoubleValueException);

#undef P
}

/* **********************************************
 * Person::is_in_infection_range() tests
 * *********************************************/
#define P(x, y, r)                                                             \
  Person { {x, y}, {0, 0}, r, Person::InfectionStatus::GREEN }

TEST(Person, is_in_infection_range_apart) {
  ASSERT_FALSE(Person::is_in_infection_range(P(0, 0, 1), P(10, 10, 1)));
  ASSERT_FALSE(Person::is_in_infection_range(P(0, 0, 1), P(10, 10, 8)));
  ASSERT_FALSE(Person::is_in_infection_range(P(-1, -1, 1), P(1, 1, 1)));
}

TEST(Person, is_in_infection_range_tangential_outside) {
  ASSERT_TRUE(Person::is_in_infection_range(P(0, 0, 0.5), P(0, 1, 0.5)));
  ASSERT_TRUE(Person::is_in_infection_range(P(0, 0, 5), P(0, 10, 5)));
  ASSERT_TRUE(Person::is_in_infection_range(P(4, 4, 2), P(4, 1, 1)));
}

TEST(Person, is_in_infection_range_overlaping) {
  ASSERT_TRUE(Person::is_in_infection_range(P(0, 0, 1), P(1, 0, 1)));
  ASSERT_TRUE(Person::is_in_infection_range(P(0, 0, 1), P(1, 1, 2)));
  ASSERT_TRUE(Person::is_in_infection_range(P(0, 0, 2), P(1, 1, 2)));
}

TEST(Person, is_in_infection_range_inside) {
  ASSERT_TRUE(Person::is_in_infection_range(P(0, 0, 1), P(0, 0, 1)));
  ASSERT_TRUE(Person::is_in_infection_range(P(0, 0, 5), P(0, 0, 1)));
  ASSERT_TRUE(Person::is_in_infection_range(P(0, 0, 5), P(1, 1, 1)));
}

#undef P

/* **********************************************
 * Person::move() tests
 * *********************************************/
void test_move(Vector2d position, Vector2d velocity, double dt,
               double city_size, Vector2d expected_position) {
  Person p{position, velocity, 1.0, Person::InfectionStatus::GREEN};

  try {
    p.move(dt, city_size);
  } catch (std::runtime_error& e) {
    FAIL() << "Caught runtime error: " << e.what() << std::endl
           << "Test failed with following values:\n"
           << "\tposition = " << position << " velocity = " << velocity
           << std::endl
           << "\tdt = " << dt << " city_size = " << city_size << std::endl
           << "\texpected_position = " << expected_position;
  }

  ASSERT_EQ(p.position(), expected_position)
    << "Test failed with following values:\n"
    << "\tposition = " << position << " velocity = " << velocity << std::endl
    << "\tdt = " << dt << " city_size = " << city_size << std::endl
    << "\texpected_position = " << expected_position;
}

void test_move_one_ten(Vector2d position, Vector2d velocity,
                       Vector2d expected_position) {
  test_move(position, velocity, 1.0, 10.0, expected_position);
}

TEST(Person, move_exceptions) {
  Person p{{0, 0}, {0, 0}, 1, Person::InfectionStatus::GREEN};

  // No Exceptions of correct arguments
  ASSERT_NO_THROW(p.move(1, 100));

  // Throw RequiredPositiveDoubleValueException on negative or 0 arguments
  ASSERT_THROW(p.move(0, 1), RequiredPositiveDoubleValueException);
  ASSERT_THROW(p.move(1, 0), RequiredPositiveDoubleValueException);
  ASSERT_THROW(p.move(-1, 1), RequiredPositiveDoubleValueException);
  ASSERT_THROW(p.move(1, -1), RequiredPositiveDoubleValueException);

  // Throw OutOfCityBoundsException when starting position is outside given
  // city_size. Do that check before checking if velocity vector is 0.
  p = {{5, 5}, {1, -1}, 1, Person::InfectionStatus::GREEN};
  ASSERT_THROW(p.move(1, 1), OutOfCityBoundsException);
  p = {{5, 5}, {0, 0}, 1, Person::InfectionStatus::GREEN};
  ASSERT_THROW(p.move(1, 1), OutOfCityBoundsException);
}

TEST(Person, move_zero_velocity) {
  Person p{{5, 5}, {0, 0}, 1, Person::InfectionStatus::GREEN};

  p.move(1, 10);
  ASSERT_EQ(p.position(), Vector2d(5, 5));
}

TEST(Person, move_normal_in_the_middle) {
  test_move_one_ten({5, 5}, {1, 2}, {6, 7});
  test_move_one_ten({5, 5}, {-2, 0}, {3, 5});
  test_move_one_ten({5, 5}, {-1, -2}, {4, 3});
}

TEST(Person, move_alongside_edge_without_bounces) {
  // rigth edge
  test_move_one_ten({10, 5}, {0, 1}, {10, 6});
  test_move_one_ten({10, 5}, {0, -2}, {10, 3});

  // top edge
  test_move_one_ten({5, 10}, {-1, 0}, {4, 10});
  test_move_one_ten({5, 10}, {2, 0}, {7, 10});

  // left edge
  test_move_one_ten({0, 5}, {0, -1}, {0, 4});
  test_move_one_ten({0, 5}, {0, 2}, {0, 7});

  // bottom edge
  test_move_one_ten({5, 0}, {1, 0}, {6, 0});
  test_move_one_ten({5, 0}, {-2, 0}, {3, 0});
}

TEST(Person, move_alongside_edge_with_bounces) {
  // rigth edge
  test_move_one_ten({10, 5}, {0, 6}, {10, 9});
  test_move_one_ten({10, 5}, {0, -7}, {10, 2});

  // top edge
  test_move_one_ten({5, 10}, {-6, 0}, {1, 10});
  test_move_one_ten({5, 10}, {7, 0}, {8, 10});

  // left edge
  test_move_one_ten({0, 5}, {0, -6}, {0, 1});
  test_move_one_ten({0, 5}, {0, 7}, {0, 8});

  // bottom edge
  test_move_one_ten({5, 0}, {6, 0}, {9, 0});
  test_move_one_ten({5, 0}, {-7, 0}, {2, 0});
}

TEST(Person, move_single_bounce_right) {
  test_move_one_ten({9, 5}, {3, 0}, {8, 5});
  test_move_one_ten({9, 5}, {2, 1}, {9, 6});
  test_move_one_ten({9, 5}, {2, -2}, {9, 3});
}

TEST(Person, move_single_bounce_top) {
  test_move_one_ten({5, 9}, {0, 3}, {5, 8});
  test_move_one_ten({5, 9}, {1, 2}, {6, 9});
  test_move_one_ten({5, 9}, {-2, 2}, {3, 9});
}

TEST(Person, move_single_bounce_left) {
  test_move_one_ten({1, 5}, {-3, 0}, {2, 5});
  test_move_one_ten({1, 5}, {-2, 1}, {1, 6});
  test_move_one_ten({1, 5}, {-2, -2}, {1, 3});
}

TEST(Person, move_single_bounce_bottom) {
  test_move_one_ten({5, 1}, {0, -3}, {5, 2});
  test_move_one_ten({5, 1}, {1, -2}, {6, 1});
  test_move_one_ten({5, 1}, {-2, -2}, {3, 1});
}

TEST(Person, move_double_bounce_top_right) {
  test_move_one_ten({8, 8}, {2.5, 5}, {9.5, 7}); // this works
  test_move_one_ten({9, 8}, {3, 3}, {8, 9});     // and this doesn't
}

TEST(Person, move_from_side_to_side) {
  Person p{{0, 5}, {10, 1}, 1, Person::InfectionStatus::GREEN};

  p.move(1, 10);
  ASSERT_EQ(p.position(), Vector2d(10, 6));
  p.move(1, 10);
  ASSERT_EQ(p.position(), Vector2d(0, 7));
  p.move(1, 10);
  ASSERT_EQ(p.position(), Vector2d(10, 8));
}

TEST(Person, move_known_past_bugs) {
  test_move({0.125, 0.25}, {0.1, 0.2}, 0.01, 0.25, {0.126, 0.248});
  test_move({0.25, -2.63678e-16}, {0.1, -0.2}, 0.01, 0.25, {0.249, 0.002});
}