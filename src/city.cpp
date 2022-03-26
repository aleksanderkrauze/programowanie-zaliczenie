#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "city.h"
#include "person.h"

/**
 * # Exceptions
 * - Throws std::invalid_argument when @dt is less than or equal to zero
 * - Throws std::invalid_argument when @city_size is less than or equal to zero
 * - Throws std::invalid_argument when @recovery_time is less than or equal to zero
 */
City::City(std::uint32_t n_iter, double dt, double city_size, double recovery_time):
	_n_iter {n_iter}, _dt {dt}, _city_size {city_size}, _recovery_time {recovery_time}, _people {}
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
	auto x = person.x();
	auto y = person.y();
	auto size = this->_city_size;

	if(!(0 <= x && x <= size)) {
		std::ostringstream s;
		s << "Person's x coordinate (x: " << x << ") is outside City's bounds [0, " << size << "]";
		throw std::out_of_range(s.str());
	}
	if(!(0 <= y && y <= this->_city_size)) {
		std::ostringstream s;
		s << "Person's y coordinate (y: " << y << ") is outside City's bounds [0, " << size << "]";
		throw std::out_of_range(s.str());
	}

	this->_people.push_back(person);
}
