#include <cmath> // M_PI const, sqrt
#include <string>
#include <vector>

#include "exceptions.h"
#include "matplotlibcpp.h"
#include "person.h"

/* **********************************************
 * Private functions of this file, not declared in person.h
 * **********************************************/

double people_distance(const Person& p1, const Person& p2) {
  auto delta_x = p1.x() - p2.x();
  auto delta_y = p1.y() - p2.y();

  return std::sqrt(delta_x * delta_x + delta_y * delta_y);
}

/* **********************************************
 * End of private functions
 * **********************************************/

std::string infection_status_to_colour(const InfectionStatus status) {
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
 * - Throws std::invalid_argument when @radius is less than zero
 */
Person::Person(const double x, const double y, const double vx, const double vy,
               const double radius, const InfectionStatus status)
    : m_x{x}, m_y{y}, m_vx{vx}, m_vy{vy}, m_radius{radius},
      m_time_of_infection{0.0}, m_infection_status{status} {
  if (radius <= 0) {
    throw RequiredPositiveDoubleValueException("radius", radius);
  }
}

double Person::x() const noexcept { return this->m_x; }

void Person::x(double x) noexcept { this->m_x = x; }

double Person::y() const noexcept { return this->m_y; }

void Person::y(double y) noexcept { this->m_y = y; }

double Person::vx() const noexcept { return this->m_vx; }

void Person::vx(double vx) noexcept { this->m_vx = vx; }

double Person::vy() const noexcept { return this->m_vy; }

void Person::vy(double vy) noexcept { this->m_vy = vy; }

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

  const std::vector<double> x = {this->m_x};
  const std::vector<double> y = {this->m_y};

  const auto radius = this->m_radius * radius_to_pixel;
  const auto circle_area = M_PI * radius * radius;

  const auto colour = infection_status_to_colour(this->m_infection_status);

  matplotlibcpp::scatter(x, y, circle_area, {{"color", colour}});
}

void Person::move(const double dt, const double city_size) {
  // TODO: implement this function
}

bool Person::is_in_infection_range(const Person& p1, const Person& p2) {
  return people_distance(p1, p2) <= (p1.radius() + p2.radius());
}