#include <cstdint>
#include <sstream>
#include <utility>
#include <vector>

#include "city.h"
#include "config.h"
#include "exceptions.h"
#include "line2d.h"
#include "person.h"
#include "vector2d.h"

/**
 * # Exceptions
 * - Throws RequiredPositiveDoubleValueException when @city_size is less than or
 * equal to zero
 * - Throws RequiredPositiveDoubleValueException when @time is less than or
 * equal to zero
 * - Throws RequiredPositiveDoubleValueException when @dt is less than or equal
 * to zero
 * - Throws RequiredPositiveDoubleValueException when @recovery_time is less
 * than or equal to zero
 */
City::City(const double city_size, const double time, const double dt,
           const double recovery_time)
    : m_city_size{city_size}, m_time{time}, m_dt{dt},
      m_recovery_time{recovery_time}, m_current_time{0.0}, m_people{} {
  if (city_size <= 0) {
    throw RequiredPositiveDoubleValueException("city_size", city_size);
  }
  if (time <= 0) {
    throw RequiredPositiveDoubleValueException("time", time);
  }
  if (dt <= 0) {
    throw RequiredPositiveDoubleValueException("dt", dt);
  }
  if (recovery_time <= 0) {
    throw RequiredPositiveDoubleValueException("recovery_time", recovery_time);
  }
}

/**
 * # Exceptions
 * - Throws OutOfCityBoundsException when @person's position is outside City's
 * size bounds
 */
void City::add_person(Person&& person) {
  if (!City::is_in_bound(person.position(), this->m_city_size)) {
    throw OutOfCityBoundsException(person, this->m_city_size);
  }

  this->m_people.push_back(std::move(person));
}

const std::vector<Person>& City::people() const noexcept {
  return this->m_people;
}

/**
 * # Exceptions
 * - Throws RequiredPositiveDoubleValueException when @city_size is less than or
 * equal to zero
 */
bool City::is_in_bound(const Vector2d& p, const double city_size) {
  if (city_size <= 0) {
    throw RequiredPositiveDoubleValueException("city_size", city_size);
  }

  const Line2d right_edge{{city_size, 0.0}, {0.0, 1.0}};
  const Line2d top_edge{{city_size, city_size}, {-1.0, 0.0}};
  const Line2d left_edge{{0.0, city_size}, {0.0, -1.0}};
  const Line2d bottom_edge{{0.0, 0.0}, {1.0, 0.0}};

  // We must check position in relation to lines, because due to floating
  // point arithmetic we could be *slightly* outside, but is reality on the
  // edge.
  return !(right_edge.point_position(p) == Line2d::PointPosition::RIGHT) &&
         !(top_edge.point_position(p) == Line2d::PointPosition::RIGHT) &&
         !(left_edge.point_position(p) == Line2d::PointPosition::RIGHT) &&
         !(bottom_edge.point_position(p) == Line2d::PointPosition::RIGHT);
}

City City::from_config(const Config& config) {
  City city{config.city_size, config.time, config.dt, config.recovery_time};

  if (config.simulation_type == Config::SimulationType::TEST) {
    Person p1{{0.1, 0.2}, {0.1, 0.2}, 0.02, Person::InfectionStatus::GREEN};
    Person p2{{0.1, 0.1}, {0.5, 0.3}, 0.05, Person::InfectionStatus::GREEN};
    Person p3{{0.2, 0.l}, {0.3, 0.6}, 0.03, Person::InfectionStatus::RED};

    city.add_person(std::move(p1));
    city.add_person(std::move(p2));
    city.add_person(std::move(p3));
  } else if (config.simulation_type == Config::SimulationType::RANDOM) {
    //
  } else if (config.simulation_type == Config::SimulationType::FILE) {
    //
  } else {
    throw std::runtime_error("Unknown simulation type");
  }

  return city;
}

void City::update_recovering() noexcept {
  for (auto& person : this->m_people) {
    if (person.infection_status() == Person::InfectionStatus::RED) {
      if (person.time_of_infection() + this->m_recovery_time <=
          this->m_current_time) {
        person.infection_status(Person::InfectionStatus::BLUE);
      }
    }
  }
}

void City::infection() noexcept {
  const size_t size = this->m_people.size();
  const auto check_and_infect = [](const auto& infected_person, auto& person) {
    if (person.infection_status() == Person::InfectionStatus::GREEN &&
        Person::is_in_infection_range(infected_person, person)) {
      person.infection_status(Person::InfectionStatus::RED);
    }
  };

  for (size_t i = 0; i < size; ++i) {
    const auto& current_person = this->m_people[i];
    if (current_person.infection_status() == Person::InfectionStatus::RED) {
      // all pople before current_person
      for (size_t j = 0; j < i; ++j) {
        check_and_infect(current_person, this->m_people[j]);
      }
    }
    // all people after current_person
    for (size_t j = i + 1; j < size; ++j) {
      check_and_infect(current_person, this->m_people[j]);
    }
  }
}

void City::move() noexcept {
  for (auto& p : this->m_people) {
    p.move(this->m_dt, this->m_city_size);
  }
}