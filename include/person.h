#ifndef PERSON_H
#define PERSON_H

#include <string>

enum class InfectionStatus {GREEN, RED, BLUE};

std::string infection_status_to_colour(const InfectionStatus);

class Person {
public:
	Person(const double, const double, const double, const double, const double, const InfectionStatus);
	Person(Person&&) = default;
	~Person() = default;

	Person& operator=(Person&&) = default;

	double x() const;
	void x(double);
	double y() const;
	void y(double);
	double vx() const;
	void vx(double);
	double vy() const;
	void vy(double);
	double radius() const;
	double time_of_infection() const;
	void time_of_infection(double);
	InfectionStatus infection_status() const;
	void infection_status(InfectionStatus);

	void draw() const;
private:
	double m_x, m_y, m_vx, m_vy, m_radius, m_time_of_infection;
	InfectionStatus m_infection_status;

	Person(const Person&) = default;
	Person& operator=(const Person&) = default;
};

#endif // PERSON_H
