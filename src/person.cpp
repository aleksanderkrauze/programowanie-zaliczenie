#include <cmath> // M_PI const, sqrt
#include <string>
#include <vector>

#include "exceptions.h"
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
 * - Throws std::invalid_argument when @radius is less than or equal to zero
 */
Person::Person(const double x, const double y, const double vx, const double vy,
               const double radius, const InfectionStatus status)
    : Person{{x, y}, {vx, vy}, radius, status} {}

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
  // TODO: implement this function
}

bool Person::is_in_infection_range(const Person& p1, const Person& p2) {
  return Vector2d::distance(p1.m_position, p2.m_position) <=
         (p1.m_radius + p2.m_radius);
}

std::string
Person::infection_status_to_colour(const Person::InfectionStatus status) {
  switch (status) {
  case InfectionStatus::GREEN:
    return "green";
  case InfectionStatus::RED:
    return "red";
  case InfectionStatus::BLUE:
    return "blue";
  }

  throw std::runtime_error("Unexpected infection status");
}
