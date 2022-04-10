#include <cmath> // M_PI const, sqrt
#include <string>
#include <vector>

#include "algebra.h"
#include "exceptions.h"
#include "matplotlibcpp.h"
#include "person.h"

std::string infection_status_to_colour(InfectionStatus status) {
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

/**
 * # Exceptions
 * - Throws std::invalid_argument when @radius is less than or equalt to zero
 */
Person::Person(Vector2d position, Vector2d velocity, double radius,
               InfectionStatus status)
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
Person::Person(double x, double y, double vx, double vy, double radius,
               InfectionStatus status)
    : Person{{x, y}, {vx, vy}, radius, status} {}

Vector2d Person::position() const noexcept { return this->m_position; }

void Person::position(Vector2d position) noexcept {
  this->m_position = position;
}

Vector2d Person::velocity() const noexcept { return this->m_velocity; }

void Person::velocity(Vector2d velocity) noexcept {
  this->m_velocity = velocity;
}

double Person::radius() const noexcept { return this->m_radius; }

double Person::time_of_infection() const noexcept {
  return this->m_time_of_infection;
}

void Person::time_of_infection(double time) noexcept {
  this->m_time_of_infection = time;
}

InfectionStatus Person::infection_status() const noexcept {
  return this->m_infection_status;
}

void Person::infection_status(InfectionStatus status) noexcept {
  this->m_infection_status = status;
}

void Person::draw() const {
  const double radius_to_pixel = 200;

  const auto [_x, _y] = this->m_position.tuple();
  const std::vector<double> x = {_x};
  const std::vector<double> y = {_y};

  const auto radius = this->m_radius * radius_to_pixel;
  const auto circle_area = M_PI * radius * radius;

  const auto colour = infection_status_to_colour(this->m_infection_status);

  matplotlibcpp::scatter(x, y, circle_area, {{"color", colour}});
}

void Person::move(double dt, double city_size) {
  // TODO: implement this function
}

bool Person::is_in_infection_range(const Person& p1, const Person& p2) {
  return Vector2d::distance(p1.m_position, p2.m_position) <=
         (p1.radius() + p2.radius());
}