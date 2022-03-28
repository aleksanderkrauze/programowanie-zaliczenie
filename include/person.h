#ifndef PERSON_H
#define PERSON_H

#include <string>

enum class InfectionStatus {GREEN, RED, BLUE};

std::string infection_status_to_colour(InfectionStatus);

class Person {
private:
	double _x, _y, _vx, _vy, _radius, _time_of_infection;
	InfectionStatus _infection_status;

	Person(const Person&) = default;
	Person& operator=(const Person&) = default;
public:
	Person(double, double, double, double, double, InfectionStatus);
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
};

#endif // PERSON_H
