#ifndef CITY_H
#define CITY_H

#include <cstdint>
#include <vector>

#include "person.h"

class City {
private:
	std::uint32_t _n_iter;
	double _dt, _city_size, _recovery_time;
	std::vector<Person> _people;
public:
	City(std::uint32_t, double, double, double);
	~City() = default;
	void add_person(Person&&);
};

#endif // CITY_H
