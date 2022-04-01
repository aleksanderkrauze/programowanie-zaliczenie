#include <exception>
#include <string>
#include <sstream>

#include "exception.h"
#include"person.h"

OutOfCityBoundsException::OutOfCityBoundsException(const char* msg): std::runtime_error {msg} {}

OutOfCityBoundsException::OutOfCityBoundsException(const Person& p, const double city_size)
{
		std::ostringstream s;
		s << "Person's coordinates (x, y) = ("
			<< p.x() << ", " << p.y()
			<< ") are outside City's bounds: [0, " << city_size << "]";

		std::runtime_error(s.str());
}
