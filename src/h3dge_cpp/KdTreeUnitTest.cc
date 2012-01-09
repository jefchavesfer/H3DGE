#include "KdTree.hh"
#include "gtest/gtest.h"

point computeKdTreeNodeExitPoint(struct KdTreeNode *node,
                                 struct Ray *ray);

void EXPECT_NEAR_POINTS(const point &p1, const point &p2) {
	EXPECT_NEAR(p1.x + 0.0, p2.x + 0.0, NUMTYPES_PRECISION);
	EXPECT_NEAR(p1.y + 0.0, p2.y + 0.0, NUMTYPES_PRECISION);
	EXPECT_NEAR(p1.z + 0.0, p2.z + 0.0, NUMTYPES_PRECISION);
}

TEST(ExitPoint, Parallel)
{
	for (int plane = 0; plane < 3; plane++) {
		struct Ray myRay = {{0, 0, 0}, {0, 0, 0}};
		myRay.start[plane] = -1;
		myRay.direction[plane] = 1;
		struct KdTreeNode myNode;
		myNode.minBounds = (point){0, 0, 0};
		myNode.maxBounds = (point){1, 1, 1};
		point expectedExitPoint = {0, 0, 0};
		expectedExitPoint[plane] = 1;

		
		EXPECT_NEAR_POINTS(computeKdTreeNodeExitPoint(&myNode, &myRay), expectedExitPoint);

		myNode.maxBounds[plane] = 0.5;
		expectedExitPoint[plane] = 0.5;

		EXPECT_NEAR_POINTS(computeKdTreeNodeExitPoint(&myNode, &myRay), expectedExitPoint);
	}
}

TEST(ExitPoint, NotParallel)
{
	struct Ray myRay = {{0, 0, 0}, {0.4, 1, 0.5}};
	myRay.direction *= 1.0 / myRay.direction.norm();
	struct KdTreeNode myNode;
	myNode.minBounds = (point){0, 0, 0};
	myNode.maxBounds = (point){1, 1, 1};
	point expectedExitPoint = {0.4, 1, 0.5};

	EXPECT_NEAR_POINTS(computeKdTreeNodeExitPoint(&myNode, &myRay), expectedExitPoint);

	myRay.start = (point){1, 1, 1};
	myRay.direction = (vector){-1, -1, -1};
	myRay.direction *= 1.0 / myRay.direction.norm();
	expectedExitPoint = (point){0, 0, 0};

	EXPECT_NEAR_POINTS(computeKdTreeNodeExitPoint(&myNode, &myRay), expectedExitPoint);
}
