#ifndef PERSON_H
#define PERSON_H

#include "plotter.h"

enum class InfectionStatus {GREEN, RED, BLUE};

class Person: public Drawable {
private:
	double _x, _y, _vx, _vy, _radius, _time_of_infection;
	InfectionStatus _infection_status;
public:
	Person(double, double, double, double, double, InfectionStatus);
	~Person() = default;
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
	void draw() const override;
};

#endif // PERSON_H
