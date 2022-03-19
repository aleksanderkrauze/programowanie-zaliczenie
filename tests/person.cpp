#include <gtest/gtest.h>

#include "person.h"

TEST(Person, getters) {
	Person p {1.0, 2.0, 3.0, -2.0, 5.0, InfectionStatus::GREEN};

	EXPECT_EQ(p.member_x(), 1.0);
	EXPECT_EQ(p.member_y(), 2.0);
	EXPECT_EQ(p.member_vx(), 3.0);
	EXPECT_EQ(p.member_vy(), -2.0);
	EXPECT_EQ(p.member_radius(), 5.0);
	EXPECT_EQ(p.member_time_of_infection(), 0.0);
	EXPECT_EQ(p.member_infection_status(), InfectionStatus::GREEN);
}

TEST(Person, setters) {
	Person p {0, 0, 0, 0, 0, InfectionStatus::GREEN};

	p.member_x(3.0);
	EXPECT_EQ(p.member_x(), 3.0);
	p.member_y(2.0);
	EXPECT_EQ(p.member_y(), 2.0);
	p.member_vx(7.0);
	EXPECT_EQ(p.member_vx(), 7.0);
	p.member_vy(-3.0);
	EXPECT_EQ(p.member_vy(), -3.0);
	p.member_time_of_infection(120.0);
	EXPECT_EQ(p.member_time_of_infection(), 120.0);
	p.member_infection_status(InfectionStatus::RED);
	EXPECT_EQ(p.member_infection_status(), InfectionStatus::RED);
}
