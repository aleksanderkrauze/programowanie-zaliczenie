#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <string>

#include "person.h"

class OutOfCityBoundsException: public std::runtime_error {
public:
	explicit OutOfCityBoundsException(const char*);
	OutOfCityBoundsException(const Person&, const double);
};

#endif // EXCEPTION_H
