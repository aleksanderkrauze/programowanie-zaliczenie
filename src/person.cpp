#include <cmath> // M_PI const
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "matplotlibcpp.h"
#include "person.h"

std::string infection_status_to_colour(InfectionStatus status) {
	switch(status) {
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
Person::Person(double x, double y, double vx, double vy, double radius, InfectionStatus status):
	m_x {x},
	m_y {y},
	m_vx {vx},
	m_vy {vy},
	m_radius {radius},
	m_time_of_infection {0.0},
	m_infection_status {status}
{
	if(radius < 0) {
		std::ostringstream s;
		s << "radius must be greater than 0 (got " << radius << ")";
		throw std::invalid_argument(s.str());
	}
}

double Person::x() const {
	return this->m_x;
}

void Person::x(double x) {
	this->m_x = x;
}

double Person::y() const {
	return this->m_y;
}

void Person::y(double y) {
	this->m_y = y;
}

double Person::vx() const {
	return this->m_vx;
}

void Person::vx(double vx) {
	this->m_vx = vx;
}

double Person::vy() const {
	return this->m_vy;
}

void Person::vy(double vy) {
	this->m_vy = vy;
}

double Person::radius() const {
	return this->m_radius;
}

double Person::time_of_infection() const {
	return this->m_time_of_infection;
}

void Person::time_of_infection(double time) {
	this->m_time_of_infection = time;
}

InfectionStatus Person::infection_status() const {
	return this->m_infection_status;
}

void Person::infection_status(InfectionStatus status) {
	this->m_infection_status = status;
}

void Person::draw() const {
	const double radius_to_pixel = 200;

	const std::vector<double> x = {this->x()};
	const std::vector<double> y = {this->y()};

	const auto radius = this->radius() * radius_to_pixel;
	const auto circle_area = M_PI * radius * radius;

	const auto colour = infection_status_to_colour(this->infection_status());

	matplotlibcpp::scatter(x, y, circle_area, {{"color", colour}}); 
}
