#include "RayTracer.hh"
#include <iostream>
using namespace std;

bool hitTriangle(const struct Ray &ray, const struct Triangle &triangle,
                 scalar &multParameter) {
	vector E1 = triangle.vertices.v2 - triangle.vertices.v1;
	vector E2 = triangle.vertices.v3 - triangle.vertices.v1;
	vector T = ray.start - triangle.vertices.v1;
	vector P = ray.direction ^ E2;
	vector Q = T ^ E1;
	scalar det = P * E1;

	if (det == 0.0)
		return false;

	scalar u = (T * P) / det;
	scalar v = (ray.direction * Q) / det;

	if (((u > 0) || isNear(u, 0))
	    && ((v > 0) || isNear(v, 0))
	    && (isWithin(0, (u + v), 1))) {
		scalar t = (E2 * Q) / det;

		if (t >= 0) {
			multParameter = t;

			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

bool RayTracer::isReflected(const struct Ray &ray) {
	const struct Triangle *triangleHit = ray.triangleHit;
	const unsigned int materialIndex = triangleHit->materialIndex;
	const struct Material &material = scene.materials[materialIndex];

	return (material.reflection > 0);
}

vector RayTracer::computeReflectedRayDirection(const struct Ray &ray) {
	const vector &I = ray.direction;
	vector N = ray.triangleHit->normal();
	if (N * I > 0)
		N = -N;

	return I - 2 * N * (I * N);
}

bool RayTracer::isRefracted(const struct Ray &ray) {
	const struct Triangle *triangleHit = ray.triangleHit;
	const unsigned int materialIndex = triangleHit->materialIndex;
	const struct Material &material = scene.materials[materialIndex];

	return (material.transmission > 0);
}

vector RayTracer::computeRefractedRayDirection(const struct Ray &ray) {
	const int medium1Index = ray.triangleHit->medium1Index;
	const int medium2Index = ray.triangleHit->medium2Index;
	const scalar n1 = ray.currentNRefraction + 0.0;
	const scalar n2 = ((scene.media[medium1Index].nRefraction == n1) ?
	                   scene.media[medium2Index].nRefraction + 0.0
	                   : scene.media[medium1Index].nRefraction + 0.0);
	const vector &i = ray.direction;
	vector n = ray.triangleHit->normal();
	vector t;

	if (n * i > 0)
		n = -n;

	if (n1 < n2) {
		t = (n1/n2)*i - ((n1/n2)*(i*n)
		                 + sqrt(1 - (n1/n2)*(n1/n2)*(1 - (i*n)*(i*n))))*n;
	} else if (n1 == n2) {
		t = ray.direction;
	} else {
		t = ray.direction;
	}

	return t;
}

bool RayTracer::findNearestIntersection(struct Ray &ray) {
	struct KdTreeNode* currentNode = ray.currentNode;
	struct Triangle* triangleHit = ray.triangleHit;
	struct Triangle *nearestIntersected = NULL;

	currentNode = kdTree.getNextNode(NULL, &ray);

	while (currentNode) {
		scalar minParam = NUMTYPES_SCALAR_MAX;
		point minIntersection = {0, 0, 0};
		bool hit = false;

		for (struct TrigRef *ref = currentNode->triangles;
		     ref; ref = ref->next) {
			scalar param;

			assert(ref->triangle);

			// Don't try to intersect with yourself...
			if (ref->triangle == triangleHit)
				continue;

			if (hitTriangle(ray, *ref->triangle, param)) {
				point intersection = ray.start + param * ray.direction;
				if (!isNear(intersection, ray.intersection)
				    && !isNear(param, minParam) && (param < minParam)) {
					hit = true;
					minParam = param;
					minIntersection = intersection;
					nearestIntersected = ref->triangle;
				}
			}
		}

		if (hit) {
			ray.intersection = minIntersection;
			ray.triangleHit = nearestIntersected;
			ray.currentNode = currentNode;
			return true;
		}

		currentNode = kdTree.getNextNode(currentNode, &ray);
	}

	triangleHit = NULL;

	return false;
}

rgb RayTracer::computeLighting(const struct Ray &ray) {
	const point &intersection = ray.intersection;
	const Triangle &triangle = *ray.triangleHit;
	rgb pixelColor = {0, 0, 0};
	vector triangleNormal = triangle.normal();

	if (triangleNormal * ray.direction > 0) {
		triangleNormal = -triangleNormal;
	}

	for (unsigned int i = 0; i < scene.nbLights; i++) {
		struct Light &light = scene.lights[i];
		struct Ray currentRay = ray;
		scalar accTransmissionCoeff = 1.0;

		currentRay.start = intersection;
		currentRay.direction = light.position - intersection;
		currentRay.direction *= 1.0 / currentRay.direction.norm();

		if (currentRay.direction * triangleNormal < 0) {
			coeff transmission = scene.materials[
				currentRay.triangleHit->materialIndex].transmission;
			accTransmissionCoeff = accTransmissionCoeff * transmission;
		}

		while (findNearestIntersection(currentRay)) {
			coeff transmission = scene.materials[
				currentRay.triangleHit->materialIndex].transmission;
			accTransmissionCoeff = accTransmissionCoeff * transmission;
			currentRay.start = currentRay.intersection;
		}

		{
			const struct Material &material = scene.materials[
				triangle.materialIndex];
			const rgb &trianglePixelColor = material.color;
			scalar coef = 1.0;
			assert(currentRay.depth <= RAY_TREE_MAX_DEPTH);

			if (ray.type == RAY_REFLECTED || ray.type == RAY_ROOT) {
				scalar lambert = (currentRay.direction * triangleNormal) * coef;
				if (lambert < 0)
					lambert = -lambert;
				lambert = lambert * accTransmissionCoeff;
				lambert = lambert * ray.amplitude;
				lambert = lambert * (1 - material.transmission);
				pixelColor += lambert * trianglePixelColor * light.color;
			} else if (ray.type == RAY_REFRACTED) {
				scalar lambert = (currentRay.direction * triangleNormal) * coef;
				if (lambert < 0)
					lambert = -lambert;
				lambert = lambert * accTransmissionCoeff;
				lambert = lambert * ray.amplitude;
				lambert = lambert * (1 - material.transmission);
				pixelColor += lambert * trianglePixelColor * light.color;
			}
		}
	}

	return pixelColor;
}

rgb RayTracer::computePixelColor(unsigned int x, unsigned int y) {
	const scalar scaleFactor = ((scene.width < scene.height) ?
	                            scene.width : scene.height);
	const scalar xOffset = (scene.width - scaleFactor) / scaleFactor + 1.0;
	const scalar yOffset = (scene.height - scaleFactor) / scaleFactor + 1.0;
	const scalar startX = 2 * (scalar)x / scaleFactor - xOffset;
	const scalar startY = -(2 * (scalar)y / scaleFactor - yOffset);
	RayFactory rayFactory;
	struct Ray *ray = rayFactory.newRay((point){startX, startY, -1.5},
	                                           (vector){0, 0, 1.0});
	rgb pixelColor = {0, 0, 0};
	ray->currentNRefraction = scene.media[0].nRefraction;

	// if (x == 319 && y == 240)
	//  	cout << "Dark pixel" << endl;

	for (; ray; ray = ray->next) {
		assert(ray != NULL);
		if (findNearestIntersection(*ray)) {
			const struct Material material = scene.materials[
				ray->triangleHit->materialIndex];

			pixelColor += computeLighting(*ray);

			if (ray->depth >= RAY_TREE_MAX_DEPTH)
				break;

			if (isRefracted(*ray)) {
				// Add refracted ray
				const int medium1Index = ray->triangleHit->medium1Index;
				const int medium2Index = ray->triangleHit->medium2Index;
				const coeff n1 = scene.media[medium1Index].nRefraction;
				const coeff n2 = scene.media[medium2Index].nRefraction;
				vector refractedDirection = computeRefractedRayDirection(*ray);
				struct Ray *refracted = rayFactory.newRay(*ray);

				refracted->start = ray->intersection;
				refracted->direction = refractedDirection;
				refracted->depth = ray->depth + 1;
				refracted->type = RAY_REFRACTED;
				refracted->amplitude = ray->amplitude * material.transmission;
				ray->next = refracted;
				if (ray->currentNRefraction == n1)
					ray->currentNRefraction = n2;
				else
					ray->currentNRefraction = n1;
			}

			if (isReflected(*ray)) {
				// Add reflected ray
				vector reflectedDirection = computeReflectedRayDirection(*ray);
				struct Ray *reflected = rayFactory.newRay(*ray);

				reflected->start = ray->intersection;
				reflected->direction = reflectedDirection;
				reflected->depth = ray->depth + 1;
				reflected->type = RAY_REFLECTED;
				reflected->amplitude = ray->amplitude * material.reflection;
				ray->next = reflected;
			}
		}
	}

	return pixelColor;
}

bool RayTracer::rayTrace(uint32_t *frameBuffer) {
	unsigned int progressBar = 1;
	const unsigned int progressBarLength = 40;

	cout << "[";
	fflush(stdout);

	for (unsigned int y = 0; y < scene.height; y++) {
		for (unsigned int x = 0; x < frameBufferWidth; x++) {
			if (x < scene.width) {
				rgb pixelColor = computePixelColor(x, y);
				unsigned char red = pixelColor.red * 255.0;
				unsigned char green = pixelColor.green * 255.0;
				unsigned char blue = pixelColor.blue * 255.0;

				frameBuffer[x + y * frameBufferWidth] = ((red << 16)
			                                             | (green << 8)
			                                             | blue);
			}
		}

		if (((progressBarLength * (y + 1)) / scene.height) >= progressBar) {
			cout << "=";
			fflush(stdout);
			progressBar++;
		}
	}

	cout << "] ";
	fflush(stdout);

	return true;
}
