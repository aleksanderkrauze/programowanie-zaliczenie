#include <cmath> // M_PI const, sqrt
#include <ostream>
#include <string>
#include <vector>

#include "exceptions.h"
#include "line2d.h"
#include "person.h"
#include "vector2d.h"

/**
 * # Exceptions
 * - Throws std::invalid_argument when @radius is less than or equalt to zero
 */
Person::Person(const Vector2d position, const Vector2d velocity,
               const double radius, const InfectionStatus status)
    : m_position{position}, m_velocity{velocity}, m_radius{radius},
      m_time_of_infection{0.0}, m_infection_status{status} {
  if (radius <= 0) {
    throw RequiredPositiveDoubleValueException("radius", radius);
  }
}

/**
 * # Exceptions
 * - Throws std::invalid_argument when @radius is less than or equalt to zero
 */
Person::Person(const Vector2d position, const Vector2d velocity,
               const double radius)
    : Person{position, velocity, radius, Person::InfectionStatus::GREEN} {}

/**
 * # Exceptions
 * - Throws std::invalid_argument when @radius is less than or equal to zero
 */
Person::Person(const double x, const double y, const double vx, const double vy,
               const double radius, const InfectionStatus status)
    : Person{{x, y}, {vx, vy}, radius, status} {}

/**
 * # Exceptions
 * - Throws std::invalid_argument when @radius is less than or equalt to zero
 */

Person::Person(const double x, const double y, const double vx, const double vy,
               const double radius)
    : Person{{x, y}, {vx, vy}, radius, Person::InfectionStatus::GREEN} {}

Vector2d Person::position() const noexcept { return this->m_position; }

void Person::position(const Vector2d position) noexcept {
  this->m_position = position;
}

Vector2d Person::velocity() const noexcept { return this->m_velocity; }

void Person::velocity(const Vector2d velocity) noexcept {
  this->m_velocity = velocity;
}

double Person::radius() const noexcept { return this->m_radius; }

double Person::time_of_infection() const noexcept {
  return this->m_time_of_infection;
}

void Person::time_of_infection(const double time) noexcept {
  this->m_time_of_infection = time;
}

Person::InfectionStatus Person::infection_status() const noexcept {
  return this->m_infection_status;
}

void Person::infection_status(const Person::InfectionStatus status) noexcept {
  this->m_infection_status = status;
}

void Person::move(const double dt, const double city_size) {
  if (dt <= 0.0) {
    throw RequiredPositiveDoubleValueException("dt", dt);
  }
  if (city_size <= 0.0) {
    throw RequiredPositiveDoubleValueException("city_size", city_size);
  }

  // If person's speed is 0 then it won't move.
  if (this->m_velocity.length() == 0.0) {
    return;
  }

  const Line2d right_edge{{city_size, 0.0}, {0.0, 1.0}};
  const Line2d top_edge{{city_size, city_size}, {-1.0, 0.0}};
  const Line2d left_edge{{0.0, city_size}, {0.0, -1.0}};
  const Line2d bottom_edge{{0.0, 0.0}, {1.0, 0.0}};

  Vector2d translation = this->m_velocity * dt;

  // Lambdas
  const auto is_in_city = [&](const auto& p) {
    return (right_edge.point_position(p) == Line2d::PointPosition::LEFT) &&
           (top_edge.point_position(p) == Line2d::PointPosition::LEFT) &&
           (left_edge.point_position(p) == Line2d::PointPosition::LEFT) &&
           (bottom_edge.point_position(p) == Line2d::PointPosition::LEFT);
  };

  const auto get_intersection = [this](const auto& edge, const auto& v) {
    const Line2d path{this->m_position, v};
    return Line2d::intersection(edge, path);
  };

  const auto is_intersection_ok = [city_size, this](const auto& intersection,
                                                    const auto& compound) {
    if (!intersection) {
      return false;
    }

    const auto vector = intersection.value();

    if (vector == this->m_position) {
      return false;
    }

    const auto val = compound(vector);
    return (0.0 <= val) && (val <= city_size);
  };

  const auto partial_move = [this, &translation](const auto intersection_point,
                                                 const auto& edge) {
    const auto to_edge = intersection_point - this->m_position;
    translation -= to_edge;
    this->m_position = intersection_point;

    translation.reflect(edge.normal());
    this->m_velocity.reflect(edge.normal());
  };

  const auto vertical = [](const auto& v) { return v.y(); };
  const auto horizontal = [](const auto& v) { return v.x(); };

  // Moving Person
  while (true) {
    const Vector2d candidate_position = this->m_position + translation;
    if (is_in_city(candidate_position)) {
      this->m_position = candidate_position;
      break;
    }

    const auto right_interscetion = get_intersection(right_edge, translation);
    const auto top_interscetion = get_intersection(top_edge, translation);
    const auto left_interscetion = get_intersection(left_edge, translation);
    const auto bottom_interscetion = get_intersection(bottom_edge, translation);

    if (is_intersection_ok(right_interscetion, vertical)) {
      partial_move(right_interscetion.value(), right_edge);
    } else if (is_intersection_ok(top_interscetion, horizontal)) {
      partial_move(top_interscetion.value(), top_edge);
    } else if (is_intersection_ok(left_interscetion, vertical)) {
      partial_move(left_interscetion.value(), left_edge);
    } else if (is_intersection_ok(bottom_interscetion, horizontal)) {
      partial_move(bottom_interscetion.value(), bottom_edge);
    } else {
      throw std::runtime_error("Person::move() unreachable condidtion!");
    }
  }
}

bool Person::is_in_infection_range(const Person& p1, const Person& p2) {
  return Vector2d::distance(p1.m_position, p2.m_position) <=
         (p1.m_radius + p2.m_radius);
}

std::ostream& operator<<(std::ostream& os,
                         const Person::InfectionStatus status) {
  switch (status) {
  case Person::InfectionStatus::GREEN:
    os << "green";
    break;
  case Person::InfectionStatus::RED:
    os << "red";
    break;
  case Person::InfectionStatus::BLUE:
    os << "blue";
    break;
  default:
    throw std::runtime_error("Unexpected infection status");
    break;
  }

  return os;
}