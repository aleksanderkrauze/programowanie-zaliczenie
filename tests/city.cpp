#include <stdexcept>

#include <gtest/gtest.h>

#include "city.h"
#include "person.h"

TEST(City, exceptions) {
	#define C(dt, size, rtime) City(0, dt, size, rtime)	

	EXPECT_NO_THROW(C(1, 1, 1));

	EXPECT_THROW(C(-1, 1, 1), std::invalid_argument);
	EXPECT_THROW(C(1, -1, 1), std::invalid_argument);
	EXPECT_THROW(C(1, 1, -1), std::invalid_argument);

	EXPECT_THROW(C(0, 1, 1), std::invalid_argument);
	EXPECT_THROW(C(1, 0, 1), std::invalid_argument);
	EXPECT_THROW(C(1, 1, 0), std::invalid_argument);
}

TEST(City, add_person) {
	#define P(x, y) Person {x, y, 0, 0, 0, InfectionStatus::GREEN}
	#define SIZE 100.0
	#define EPS 0.1

	City c = {0, 1, SIZE, 1};

	// middle
	EXPECT_NO_THROW(c.add_person(P(SIZE/2, SIZE/2)));

	// sides
	EXPECT_NO_THROW(c.add_person(P(SIZE/2, 0.0)));
	EXPECT_NO_THROW(c.add_person(P(SIZE, SIZE/2)));
	EXPECT_NO_THROW(c.add_person(P(SIZE/2, SIZE)));
	EXPECT_NO_THROW(c.add_person(P(0.0, SIZE/2)));

	// corners
	EXPECT_NO_THROW(c.add_person(P(0.0, 0.0)));
	EXPECT_NO_THROW(c.add_person(P(SIZE, 0.0)));
	EXPECT_NO_THROW(c.add_person(P(SIZE, SIZE)));
	EXPECT_NO_THROW(c.add_person(P(0.0, SIZE)));

	// outside (bottom left corner going counter-clockwise)
	EXPECT_THROW(c.add_person(P(-EPS, -EPS)), std::out_of_range);
	EXPECT_THROW(c.add_person(P(SIZE/2, -EPS)), std::out_of_range);
	EXPECT_THROW(c.add_person(P(SIZE + EPS, -EPS)), std::out_of_range);
	EXPECT_THROW(c.add_person(P(SIZE + EPS, SIZE/2)), std::out_of_range);
	EXPECT_THROW(c.add_person(P(SIZE + EPS, SIZE + EPS)), std::out_of_range);
	EXPECT_THROW(c.add_person(P(SIZE/2, SIZE + EPS)), std::out_of_range);
	EXPECT_THROW(c.add_person(P(-EPS, SIZE + EPS)), std::out_of_range);
	EXPECT_THROW(c.add_person(P(-EPS, SIZE/2)), std::out_of_range);
}
