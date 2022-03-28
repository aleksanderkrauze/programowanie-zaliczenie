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

	City(const City&) = default;
	City& operator=(const City&) = default;
public:
	City(std::uint32_t, double, double, double);
	City(City&&) = default;
	~City() = default;

	City& operator=(City&&) = default;

	void add_person(Person&&);
	const std::vector<Person>& people() const;
};

#endif // CITY_H
