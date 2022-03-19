#include "person.h"

Person::Person(double x, double y, double vx, double vy, double r, InfectionStatus status):
	x {x}, y {y}, vx {vx}, vy {vy}, radius {r}, infection_status {status}, time_of_infection {0.0}
{
	// TODO: assert or throw and exeption if r < 0.0;
}

double Person::member_x() const {
	return x;
}

void Person::member_x(double x) {
	this->x = x;
}

double Person::member_y() const {
	return y;
}

void Person::member_y(double y) {
	this->y = y;
}

double Person::member_vx() const {
	return vx;
}

void Person::member_vx(double vx) {
	this->vx = vx;
}

double Person::member_vy() const {
	return vy;
}

void Person::member_vy(double vy) {
	this->vy = vy;
}

double Person::member_radius() const {
	return radius;
}

double Person::member_time_of_infection() const {
	return time_of_infection;
}

void Person::member_time_of_infection(double time) {
	this->time_of_infection = time;
}

InfectionStatus Person::member_infection_status() const {
	return infection_status;
}

void Person::member_infection_status(InfectionStatus status) {
	this->infection_status = status;
}

void Person::draw() const {
	// TODO: write drawing function
}
