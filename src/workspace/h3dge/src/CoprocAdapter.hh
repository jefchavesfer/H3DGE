#ifndef _COPROCADAPTER_HH_
#define _COPROCADAPTER_HH_

#include "LinAlg.hh"
#include "Ray.hh"
#include "h3dge_coproc.h"
#include "Scene.hh"

class CoprocAdapter {
public:
	void setRay(const struct Ray &ray);
	void setTriangle(const struct Triangle &triangle);

	bool isIntersected();
	point getIntersection();

	bool isReflected();
	vector getReflectedDirection();

	bool isRefracted();
	vector getRefractedDirection();
};

#endif
