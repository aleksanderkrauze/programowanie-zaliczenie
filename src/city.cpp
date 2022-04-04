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
    : m_n_iter{n_iter}, m_dt{dt}, m_city_size{city_size},
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
