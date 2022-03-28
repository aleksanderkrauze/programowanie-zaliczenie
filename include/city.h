#ifndef CITY_H
#define CITY_H

#include <cstdint>
#include <vector>

#include "person.h"

class City {
public:
	City(const std::uint32_t, const double, const double, const double);
	City(City&&) = default;
	~City() = default;

	City& operator=(City&&) = default;

	void add_person(Person&&);
	const std::vector<Person>& people() const;
private:
	std::uint32_t m_n_iter;
	double m_dt, m_city_size, m_recovery_time;
	std::vector<Person> m_people;

	City(const City&) = default;
	City& operator=(const City&) = default;
};

#endif // CITY_H
