#include "RayTracer.h"
#include "gtest/gtest.h"

TEST(HitTriangle, NotHit)
{
	ray myRay = {{0, 0, 0}, {1, 0, 0}};
	triangle myTriangle = {0, {1, 1, 1}, {0, 0, 0}, {0, 0, 1},
				     {0, 1, 1}, {1, 0, 1}, 0};
	coord myCoord;

	EXPECT_EQ(0, hitTriangle(&myRay, &myTriangle, &myCoord));
}

TEST(HitTriangle, Hit)
{
	ray myRay = {{0, 0, 0}, {0, 0, 1}};
	triangle myTriangle = {0, {1, 1, 1}, {0, 0, 0}, {0, 0, 1},
				     {0, 1, 1}, {1, 0, 1}, 0};
	coord myCoord;

	EXPECT_EQ(1, hitTriangle(&myRay, &myTriangle, &myCoord));
}
