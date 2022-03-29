#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <vector>

#include "city.h"
#include "person.h"

/**
 * # Exceptions
 * - Throws std::invalid_argument when @dt is less than or equal to zero
 * - Throws std::invalid_argument when @city_size is less than or equal to zero
 * - Throws std::invalid_argument when @recovery_time is less than or equal to zero
 */
City::City
(
	const std::uint32_t n_iter,
	const double dt,
	const double city_size,
	const double recovery_time
):
	m_n_iter {n_iter},
	m_dt {dt},
	m_city_size {city_size},
	m_recovery_time {recovery_time},
	m_people {}
{
	if(dt <= 0) {
		std::ostringstream s;
		s << "dt must be greater than 0 (got " << dt << ")";
		throw std::invalid_argument(s.str());
	}
	if(city_size <= 0) {
		std::ostringstream s;
		s << "city_size must be greater than 0 (got " << city_size << ")";
		throw std::invalid_argument(s.str());
	}
	if(recovery_time <= 0) {
		std::ostringstream s;
		s << "recovery_time must be greater than 0 (got " << recovery_time << ")";
		throw std::invalid_argument(s.str());
	}
}

/**
 * # Exceptions
 * - Throws std::out_of_range when @person's position is outside City's size bounds
 */
void City::add_person(Person&& person) {
	const auto x = person.x();
	const auto y = person.y();
	const auto size = this->m_city_size;

	if(!(0 <= x && x <= size)) {
		std::ostringstream s;
		s << "Person's x coordinate (x: " << x << ") is outside City's bounds [0, " << size << "]";
		throw std::out_of_range(s.str());
	}
	if(!(0 <= y && y <= size)) {
		std::ostringstream s;
		s << "Person's y coordinate (y: " << y << ") is outside City's bounds [0, " << size << "]";
		throw std::out_of_range(s.str());
	}

	this->m_people.push_back(std::move(person));
}

const std::vector<Person>& City::people() const noexcept {
	return this->m_people;
}
