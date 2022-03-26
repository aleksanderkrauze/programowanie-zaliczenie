#include <sstream>
#include <stdexcept>

#include "person.h"

/**
 * # Exceptions
 * - Throws std::invalid_argument when @radius is less than zero
 */
Person::Person(double x, double y, double vx, double vy, double radius, InfectionStatus status):
	_x {x}, _y {y}, _vx {vx}, _vy {vy}, _radius {radius}, _time_of_infection {0.0}, _infection_status {status}
{
	if(radius < 0) {
		std::ostringstream s;
		s << "radius must be greater than 0 (got " << radius << ")";
		throw std::invalid_argument(s.str());
	}
}

double Person::x() const {
	return this->_x;
}

void Person::x(double x) {
	this->_x = x;
}

double Person::y() const {
	return this->_y;
}

void Person::y(double y) {
	this->_y = y;
}

double Person::vx() const {
	return this->_vx;
}

void Person::vx(double vx) {
	this->_vx = vx;
}

double Person::vy() const {
	return this->_vy;
}

void Person::vy(double vy) {
	this->_vy = vy;
}

double Person::radius() const {
	return this->_radius;
}

double Person::time_of_infection() const {
	return this->_time_of_infection;
}

void Person::time_of_infection(double time) {
	this->_time_of_infection = time;
}

InfectionStatus Person::infection_status() const {
	return this->_infection_status;
}

void Person::infection_status(InfectionStatus status) {
	this->_infection_status = status;
}

void Person::draw() const {
	// TODO: write drawing function
}
