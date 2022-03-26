#include <stdexcept>

#include <gtest/gtest.h>

#include "city.h"

TEST(City, exceptions) {
	#define C(dt, size, rtime) City(0, dt, size, rtime)	

	EXPECT_NO_THROW(C(1, 1, 1));
	EXPECT_NO_THROW(C(0, 0, 0));

	EXPECT_THROW(C(-1, 1, 1), std::invalid_argument);
	EXPECT_THROW(C(1, -1, 1), std::invalid_argument);
	EXPECT_THROW(C(1, 1, -1), std::invalid_argument);
}
