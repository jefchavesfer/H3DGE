#include "Ray.hh"
#include <cstdlib>
using namespace std;

struct Ray* RayFactory::newRay(const point &start, const vector &direction) {
	if (nextFreeRayIndex == RAY_MAX_COUNT) {
		exit(EXIT_FAILURE);
	} else {
		struct Ray *ray = &RayPool[nextFreeRayIndex++];
		memset(ray, 0, sizeof (struct Ray));
		ray->start = start;
		ray->direction = direction;
		ray->amplitude = 1.0;
		return ray;
	}
}

struct Ray* RayFactory::newRay(const struct Ray &ray) {
	if (nextFreeRayIndex == RAY_MAX_COUNT) {
		exit(EXIT_FAILURE);
	} else {
		struct Ray *newRay = &RayPool[nextFreeRayIndex++];
		*newRay = ray;
		return newRay;
	}
}
