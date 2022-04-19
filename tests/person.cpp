#include <chrono>
#include <random>
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
  // top first
  test_move_one_ten({8, 8}, {2.5, 5}, {9.5, 7});
  test_move_one_ten({9, 9}, {2, 1 + 2.0 / 3.0}, {9 + 1.0 / 3.0, 6});

  // rigth first
  test_move_one_ten({9, 8}, {3, 3}, {8, 9});
  test_move_one_ten({10, 9}, {5, 1.25}, {5, 9.75});
}

TEST(Person, move_double_bounce_top_left) {
  // top first
  test_move_one_ten({1, 10}, {-1.5, 3}, {0.5, 7});
  test_move_one_ten({3, 8}, {-5, 5}, {2, 7});

  // left first
  test_move_one_ten({1, 9}, {-4, 2}, {3, 9});
  test_move_one_ten({3, 8}, {-9, 3}, {6, 9});
}

TEST(Person, move_double_bounce_bottom_left) {
  // bottom first
  test_move_one_ten({2, 1}, {-3, -6}, {1, 5});
  test_move_one_ten({5, 1}, {-7.5, -3}, {2.5, 2});

  // left first
  test_move_one_ten({2, 1}, {-4, -4}, {4, 2});
  test_move_one_ten({1, 1}, {-3, -1}, {2, 1.0 / 3.0});
}

TEST(Person, move_double_bounce_bottom_right) {
  // bottom first
  test_move_one_ten({8, 1}, {2.5, -2.5}, {9.5, 1.5});
  test_move_one_ten({9, 1}, {2, -4}, {9, 3});

  // right first
  test_move_one_ten({10, 2}, {1.5, -3}, {8.5, 1});
  test_move_one_ten({8, 2}, {5, -3}, {1.0 / 3.0, 7});
}

TEST(Person, move_corner_top_right) {
  test_move_one_ten({8, 8}, {3, 3}, {9, 9});
  test_move_one_ten({9, 8}, {2, 4}, {9, 8});
  test_move_one_ten({7, 9}, {9, 3}, {4, 8});
}

TEST(Person, move_corner_top_left) {
  test_move_one_ten({1, 9}, {-3, 3}, {2, 8});
  test_move_one_ten({1, 5}, {-1.2, 6}, {0.2, 9});
  test_move_one_ten({2, 9}, {-4, 2}, {2, 9});
}

TEST(Person, move_corner_bottom_left) {
  test_move_one_ten({1, 1}, {-2, -2}, {1, 1});
  test_move_one_ten({1, 2}, {-1.5, -3}, {0.5, 1});
  test_move_one_ten({4, 1}, {-6, -1.5}, {2, 0.5});
}

TEST(Person, move_corner_bottom_right) {
  test_move_one_ten({7, 3}, {4, -4}, {9, 1});
  test_move_one_ten({7, 2}, {4.5, -3}, {8.5, 1});
  test_move_one_ten({8, 4}, {3, -6}, {9, 2});
}

TEST(Person, move_from_edge_to_edge) {
  Person p{{0, 5}, {10, 1}, 1, Person::InfectionStatus::GREEN};

  p.move(1, 10);
  ASSERT_EQ(p.position(), Vector2d(10, 6));
  p.move(1, 10);
  ASSERT_EQ(p.position(), Vector2d(0, 7));
  p.move(1, 10);
  ASSERT_EQ(p.position(), Vector2d(10, 8));
}

TEST(Person, move_from_corner_to_corner) {
  test_move_one_ten({0, 0}, {11, 11}, {9, 9});
  test_move_one_ten({10, 10}, {-20, -20}, {10, 10});
  test_move_one_ten({10, 0}, {-15, 15}, {5, 5});
  test_move_one_ten({0, 10}, {25, -25}, {5, 5});
}

TEST(Person, move_reverse_fuzzy_testing) {
  const auto N = 1000;
  const auto dt = 1.0;
  const auto city_size = 10.0;

  const unsigned seed =
    std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator{seed};
  std::uniform_real_distribution<double> position_distribution{0, city_size};
  std::uniform_real_distribution<double> velocity_distribution{0,
                                                               city_size / 2.0};

  const auto get_position = [&generator, &position_distribution]() {
    const auto x = position_distribution(generator);
    const auto y = position_distribution(generator);

    return Vector2d{x, y};
  };

  const auto get_velocity = [&generator, &velocity_distribution]() {
    const auto x = velocity_distribution(generator);
    const auto y = velocity_distribution(generator);

    return Vector2d{x, y};
  };

  Vector2d starting_position, starting_velocity;

  for (auto i = 0; i < N; ++i) {
    try {
      starting_position = get_position();
      starting_velocity = get_velocity();

      Person p1{starting_position, starting_velocity, 1.0,
                Person::InfectionStatus::GREEN};
      p1.move(dt, city_size);

      const auto final_position = p1.position();
      const auto final_velocity = p1.velocity();

      Person p2{final_position, -final_velocity, 1.0,
                Person::InfectionStatus::GREEN};
      p2.move(dt, city_size);

      ASSERT_EQ(std::make_pair(p2.position(), p2.velocity()),
                std::make_pair(starting_position, -starting_velocity))
        << "Reverse fuzzy test failed for following inputs:" << std::endl
        << "\tdt = " << dt << " city_size = " << city_size << std::endl
        << "\tstarting_position = " << starting_position << std::endl
        << "\tstarting_velocity = " << starting_velocity << std::endl;
    } catch (std::runtime_error& e) {
      FAIL() << "Caught runtime error: " << e.what() << std::endl
             << "Reverse fuzzy test failed for following inputs:" << std::endl
             << "\tdt = " << dt << " city_size = " << city_size << std::endl
             << "\tstarting_position = " << starting_position << std::endl
             << "\tstarting_velocity = " << starting_velocity << std::endl;
    }
  }
}

TEST(Person, move_known_past_bugs) {
  test_move({0.125, 0.25}, {0.1, 0.2}, 0.01, 0.25, {0.126, 0.248});
  test_move({0.25, -2.63678e-16}, {0.1, -0.2}, 0.01, 0.25, {0.249, 0.002});
}