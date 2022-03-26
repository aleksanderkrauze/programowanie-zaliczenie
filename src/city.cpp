#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "city.h"
#include "person.h"

/**
 * # Exeptions
 * - Throws std::invalid_argument when @dt is less than zero
 * - Throws std::invalid_argument when @city_size is less than zero
 * - Throws std::invalid_argument when @recovery_time is less than zero
 */
City::City(std::uint32_t n_iter, double dt, double city_size, double recovery_time):
	_n_iter {n_iter}, _dt {dt}, _city_size {city_size}, _recovery_time {recovery_time}, _people {}
{
	if(dt < 0) {
		std::ostringstream s;
		s << "dt must be greater than 0 (got " << dt << ")";
		throw std::invalid_argument(s.str());
	}
	if(city_size < 0) {
		std::ostringstream s;
		s << "city_size must be greater than 0 (got " << city_size << ")";
		throw std::invalid_argument(s.str());
	}
	if(recovery_time < 0) {
		std::ostringstream s;
		s << "recovery_time must be greater than 0 (got " << recovery_time << ")";
		throw std::invalid_argument(s.str());
	}
}
