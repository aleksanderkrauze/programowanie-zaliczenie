#include <utility>

#include <gtest/gtest.h>

#include "person.h"
#include "exceptions.h"

TEST(InfectionStatus, infection_status_to_colour) {
	EXPECT_STREQ(infection_status_to_colour(InfectionStatus::GREEN).c_str(), "green");
	EXPECT_STREQ(infection_status_to_colour(InfectionStatus::RED).c_str(), "red");
	EXPECT_STREQ(infection_status_to_colour(InfectionStatus::BLUE).c_str(), "blue");
}

TEST(Person, getters) {
	const Person p {1.0, 2.0, 3.0, -2.0, 5.0, InfectionStatus::GREEN};

	EXPECT_EQ(p.x(), 1.0);
	EXPECT_EQ(p.y(), 2.0);
	EXPECT_EQ(p.vx(), 3.0);
	EXPECT_EQ(p.vy(), -2.0);
	EXPECT_EQ(p.radius(), 5.0);
	EXPECT_EQ(p.time_of_infection(), 0.0);
	EXPECT_EQ(p.infection_status(), InfectionStatus::GREEN);
}

TEST(Person, setters) {
	Person p {0, 0, 0, 0, 1, InfectionStatus::GREEN};

	p.x(3.0);
	EXPECT_EQ(p.x(), 3.0);
	p.y(2.0);
	EXPECT_EQ(p.y(), 2.0);
	p.vx(7.0);
	EXPECT_EQ(p.vx(), 7.0);
	p.vy(-3.0);
	EXPECT_EQ(p.vy(), -3.0);
	p.time_of_infection(120.0);
	EXPECT_EQ(p.time_of_infection(), 120.0);
	p.infection_status(InfectionStatus::RED);
	EXPECT_EQ(p.infection_status(), InfectionStatus::RED);
}

TEST(Person, exceptions) {
	#define P(r) Person(0, 0, 0, 0, r, InfectionStatus::GREEN)	

	EXPECT_NO_THROW(P(10.0));

	EXPECT_THROW(P(0), RequiredPositiveDoubleValueException);
	EXPECT_THROW(P(-0.001), RequiredPositiveDoubleValueException);
	EXPECT_THROW(P(-0.5), RequiredPositiveDoubleValueException);
	EXPECT_THROW(P(-5), RequiredPositiveDoubleValueException);

	#undef P
}

/* **********************************************
 * Person::move() tests
 * *********************************************/
void test_move(double x, double y, double vx, double vy, double xdt, double ydt, double dt, double city_size) {
	Person p {x, y, vx, vy, 1.0, InfectionStatus::GREEN};
	p.move(dt, city_size);

	EXPECT_EQ(std::make_pair(p.x(), p.y()), std::make_pair(xdt, ydt))
		<< "Test failed with following values: "
		<< "x=" << x
		<< ", y=" << y
		<< ", vx=" << vx
		<< ", vy=" << vy
		<< ", xdt=" << xdt
		<< ", ydt=" << ydt
		<< ", city_size=" << city_size;
}

TEST(Person_move, normal_in_the_middle) {
	#define dt 1.0
	#define SIZE 10.0

	test_move(5, 5, 1, 2, 6, 7, dt, SIZE);
	test_move(5, 5, -2, 0, 3, 5, dt, SIZE);
	test_move(5, 5, -1, -2, 4, 3, dt, SIZE);

	#undef dt
	#undef SIZE
}
