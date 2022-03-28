#include <stdexcept>

#include <gtest/gtest.h>

#include "person.h"

TEST(Person, getters) {
	Person p {1.0, 2.0, 3.0, -2.0, 5.0, InfectionStatus::GREEN};

	EXPECT_EQ(p.x(), 1.0);
	EXPECT_EQ(p.y(), 2.0);
	EXPECT_EQ(p.vx(), 3.0);
	EXPECT_EQ(p.vy(), -2.0);
	EXPECT_EQ(p.radius(), 5.0);
	EXPECT_EQ(p.time_of_infection(), 0.0);
	EXPECT_EQ(p.infection_status(), InfectionStatus::GREEN);
}

TEST(Person, setters) {
	Person p {0, 0, 0, 0, 0, InfectionStatus::GREEN};

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
	#define P(x) Person(0, 0, 0, 0, x, InfectionStatus::GREEN)	

	EXPECT_NO_THROW(P(10.0));
	EXPECT_NO_THROW(P(0));

	EXPECT_THROW(P(-0.001), std::invalid_argument);
	EXPECT_THROW(P(-0.5), std::invalid_argument);
	EXPECT_THROW(P(-5), std::invalid_argument);

	#undef P
}

TEST(InfectionStatus, infection_status_to_colour) {
	EXPECT_STREQ(infection_status_to_colour(InfectionStatus::GREEN).c_str(), "green");
	EXPECT_STREQ(infection_status_to_colour(InfectionStatus::RED).c_str(), "red");
	EXPECT_STREQ(infection_status_to_colour(InfectionStatus::BLUE).c_str(), "blue");
}
