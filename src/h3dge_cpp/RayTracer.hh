#ifndef _RAYTRACER_HH_
#define _RAYTRACER_HH_

#include <stdint.h>
#include "Scene.hh"
#include "KdTree.hh"
#include "Ray.hh"

class RayTracer {
private:
	Scene &scene;
	KdTree kdTree;
	unsigned int frameBufferWidth;

	rgb computePixelColor(unsigned int x, unsigned int y);
	bool findNearestIntersection(struct Ray &ray);
	rgb computeLighting(const struct Ray &ray);

	bool isReflected(const struct Ray &ray);
	vector computeReflectedRayDirection(const struct Ray &ray);

	bool isRefracted(const struct Ray &ray);
	vector computeRefractedRayDirection(const struct Ray &ray);

public:
	explicit RayTracer(Scene &_scene, unsigned int _frameBufferWidth) :
		scene(_scene), kdTree(_scene), frameBufferWidth(_frameBufferWidth) {};
	bool rayTrace(uint32_t *frameBuffer);
};

#endif
