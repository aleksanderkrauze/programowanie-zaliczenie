#include <cstdint>
#include <sstream>
#include <utility>
#include <vector>

#include "city.h"
#include "exceptions.h"
#include "person.h"

/**
 * # Exceptions
 * - Throws RequiredPositiveDoubleValueException when @dt is less than or equal
 * to zero
 * - Throws RequiredPositiveDoubleValueException when @city_size is less than or
 * equal to zero
 * - Throws RequiredPositiveDoubleValueException when @recovery_time is less
 * than or equal to zero
 */
City::City(const std::uint32_t n_iter, const double dt, const double city_size,
           const double recovery_time)
    : m_n_iter{n_iter}, m_time{0.0}, m_dt{dt}, m_city_size{city_size},
      m_recovery_time{recovery_time}, m_people{} {
  if (dt <= 0) {
    throw RequiredPositiveDoubleValueException("dt", dt);
  }
  if (city_size <= 0) {
    throw RequiredPositiveDoubleValueException("city_size", city_size);
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
  if (!this->is_in_bound(person)) {
    throw OutOfCityBoundsException(person, this->m_city_size);
  }

  this->m_people.push_back(std::move(person));
}

const std::vector<Person>& City::people() const noexcept {
  return this->m_people;
}

bool City::is_in_bound(const Person& person) const noexcept {
  const auto x = person.x();
  const auto y = person.y();
  const auto size = this->m_city_size;

  return (0 <= x && x <= size) && (0 <= y && y <= size);
}

void City::update_recovering() noexcept {
  for (auto& person : this->m_people) {
    if (person.infection_status() == Person::InfectionStatus::RED) {
      if (person.time_of_infection() + this->m_recovery_time <= this->m_time) {
        person.infection_status(Person::InfectionStatus::BLUE);
      }
    }
  }
}

void City::infection() noexcept {
  const size_t size = this->m_people.size();

  for (size_t i = 0; i < size; ++i) {
    const auto& current_person = this->m_people[i];
    if (current_person.infection_status() == Person::InfectionStatus::RED) {
      // all pople before current_person
      for (size_t j = 0; j < i; ++j) {
        auto& person = this->m_people[j];
        if (person.infection_status() == Person::InfectionStatus::GREEN &&
            Person::is_in_infection_range(current_person, person)) {
          person.infection_status(Person::InfectionStatus::RED);
        }
      }

      // all people after current_person
      for (size_t j = i + 1; j < size; ++j) {
        auto& person = this->m_people[j];
        if (person.infection_status() == Person::InfectionStatus::GREEN &&
            Person::is_in_infection_range(current_person, person)) {
          person.infection_status(Person::InfectionStatus::RED);
        }
      }
    }
  }
}