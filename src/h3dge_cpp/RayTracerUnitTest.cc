#include "RayTracer.hh"
#include "gtest/gtest.h"

bool hitTriangle(const struct Ray &ray, const struct Triangle &triangle,
                 scalar &multParameter);

TEST(HitTriangle, NotHit)
{
	struct Ray myRay = {{0, 0, 0}, {1, 0, 0}};
	struct Triangle myTriangle = {{{0, 0, 1}, {0, 1, 1}, {1, 0, 1}}};
	scalar multParameter;

	EXPECT_EQ(false, hitTriangle(myRay, myTriangle, multParameter));
}

TEST(HitTriangle, Hit)
{
	struct Ray myRay = {{0, 0, 0}, {0, 0, 1}};
	struct Triangle myTriangle = {{{0, 0, 1}, {0, 1, 1}, {1, 0, 1}}};
	scalar multParameter;

	EXPECT_EQ(true, hitTriangle(myRay, myTriangle, multParameter));
}
