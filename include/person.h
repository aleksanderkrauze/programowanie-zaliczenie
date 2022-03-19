#ifndef PERSON_H
#define PERSON_H

#include "plotter.h"

enum class InfectionStatus {GREEN, RED, BLUE};

class Person: public Drawable {
private:
	double x, y, vx, vy, radius, time_of_infection;
	InfectionStatus infection_status;
public:
	Person(double x, double y, double vx, double vy, double r, InfectionStatus status);
	~Person() = default;
	double member_x() const;
	void member_x(double);
	double member_y() const;
	void member_y(double);
	double member_vx() const;
	void member_vx(double);
	double member_vy() const;
	void member_vy(double);
	double member_radius() const;
	double member_time_of_infection() const;
	void member_time_of_infection(double);
	InfectionStatus member_infection_status() const;
	void member_infection_status(InfectionStatus);
	void draw() const override;
};

#endif // PERSON_H
