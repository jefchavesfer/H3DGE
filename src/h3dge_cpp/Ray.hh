#ifndef _RAY_HH_
#define _RAY_HH_

#include "LinAlg.hh"

#define RAY_TREE_MAX_DEPTH 5
#define RAY_MAX_COUNT ((1 << (RAY_TREE_MAX_DEPTH + 1)) - 1)

enum RayType {
	RAY_ROOT = 0,
	RAY_REFLECTED,
	RAY_REFRACTED,
};

struct Ray {
	point start;
	vector direction;

	struct Ray *next;
	uint32_t depth;

	enum RayType type;
	struct KdTreeNode *currentNode;
	coeff currentNRefraction;
	coeff amplitude;

	struct Triangle *triangleHit;
	point intersection;
};

class RayFactory {
private:
	struct Ray RayPool[RAY_MAX_COUNT];
	int nextFreeRayIndex;
public:
	RayFactory() : nextFreeRayIndex(0) {};
	struct Ray *newRay(const point &start, const vector &direction);
	struct Ray *newRay(const struct Ray &ray);
};

#endif
