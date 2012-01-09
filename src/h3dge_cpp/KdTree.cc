#include "KdTree.hh"
#include <cstdlib>
#include <iostream>
#include <set>
using namespace std;

struct KdTreeNode* KdTree::newNode(const point &minBounds,
                                    const point &maxBounds) {
	if (nextFreeNodeIndex == KDTREE_MAX_NODES) {
		cerr << "Error: could not allocate a new KdTreeNode." << endl;
		exit(EXIT_FAILURE);
	} else {
		struct KdTreeNode *node = &NodePool[nextFreeNodeIndex++];
		memset(node, 0, sizeof (struct KdTreeNode));
		node->minBounds = minBounds;
		node->maxBounds = maxBounds;
		return node;
	}
}

struct TrigRef* KdTree::newRef(struct Triangle *triangle) {
	struct TrigRef *ref;

	if (nextFreeRefIndex == KDTREE_MAX_TRIG_REFS) {
		cerr << "Warning: could not allocate a new TrigRef from the RefPool."
		     << " Trying to allocate memory from the heap." << endl;
		ref = new TrigRef;
	} else {
		ref = &RefPool[nextFreeRefIndex++];
	}

	memset(ref, 0, sizeof (struct TrigRef));
	ref->triangle = triangle;
	return ref;
}

int largestAxis(struct KdTreeNode &node) {
	vector edgesLength = node.maxBounds - node.minBounds;
	coord xLength = edgesLength.x;
	coord yLength = edgesLength.y;
	coord zLength = edgesLength.z;

	assert(xLength >= 0);
	assert(yLength >= 0);
	assert(zLength >= 0);

	if ((xLength >= yLength) && (xLength >= zLength))
		return LINALG_AXIS_X;
	else if (yLength >= zLength)
		return LINALG_AXIS_Y;
	else
		return LINALG_AXIS_Z;
}

scalar computeNodeArea(struct KdTreeNode &node) {
	vector edgesLength = node.maxBounds - node.minBounds;
	coord xLength = edgesLength.x;
	coord yLength = edgesLength.y;
	coord zLength = edgesLength.z;

	assert(xLength >= 0);
	assert(yLength >= 0);
	assert(zLength >= 0);

	return 2 * (xLength * yLength
	            + xLength * zLength
	            + yLength * zLength);
}

scalar vertexCost(point &vertex, int splitAxis, scalar nodeArea,
                  struct KdTreeNode &node) {
	assert(node.triangles);
	assert((node.minBounds[splitAxis] <= vertex[splitAxis])
	       && (vertex[splitAxis] <= node.maxBounds[splitAxis]));
	assert(nodeArea != 0);

	struct KdTreeNode leftChild = node;
	struct KdTreeNode rightChild = node;
	leftChild.maxBounds[splitAxis] = vertex[splitAxis];
	rightChild.minBounds[splitAxis] = vertex[splitAxis];
	scalar leftChildArea = computeNodeArea(leftChild);
	scalar rightChildArea = computeNodeArea(rightChild);

	leftChild.nbTriangles = 0;
	rightChild.nbTriangles = 0;

	for (struct TrigRef *ref = node.triangles; ref; ref = ref->next) {
		int verticesToLeft = 0;
		int verticesToRight = 0;

		assert(ref->triangle);

		for (int i = 0; i < 3; i++) {
			if (ref->triangle->vertices[i][splitAxis] < vertex[splitAxis])
				verticesToLeft++;
			else
				verticesToRight++;
		}

		if (verticesToLeft > 0)
			leftChild.nbTriangles++;

		if (verticesToRight > 0)
			rightChild.nbTriangles++;
	}

	if ((leftChild.nbTriangles == 0)
	    || (rightChild.nbTriangles == 0)
	    || (computeNodeArea(leftChild) == 0)
	    || (computeNodeArea(rightChild) == 0))
		return NUMTYPES_SCALAR_MAX;

	return ((1.0 / nodeArea)
	        * (leftChildArea * leftChild.nbTriangles
	           + rightChildArea * rightChild.nbTriangles));
}

void KdTree::splitKdTreeNode(struct KdTreeNode &node) {
	int splitAxis = largestAxis(node);
	scalar minCost = NUMTYPES_SCALAR_MAX;
	coord minCostCoord = 0;
	struct TrigRef *trigRef = node.triangles;
	scalar nodeArea = computeNodeArea(node);
	set<coord> splitCoordCandidates;

	assert(trigRef);
	assert((splitAxis >= 0) && (splitAxis < 3));
	assert(node.nbTriangles > 1);

	for (; trigRef; trigRef = trigRef->next) {
		assert(trigRef);
		for (int i = 0; i < 3; i++) {
			point &vertex = trigRef->triangle->vertices[i];

			if (!((node.minBounds[splitAxis] <= vertex[splitAxis])
			      && (vertex[splitAxis] <= node.maxBounds[splitAxis])))
				continue;

			set<coord>::iterator it = splitCoordCandidates.find(vertex[splitAxis]);

			if (it != splitCoordCandidates.end())
				continue;
			else
				splitCoordCandidates.insert(vertex[splitAxis]);

			scalar vCost = vertexCost(vertex, splitAxis, nodeArea, node);
			assert(vCost >= 0);

			if (vCost < minCost) {
				minCost = vCost;
				minCostCoord = vertex[splitAxis];
			}
		}
	}

	if (node.parent && ((node.parent->splitCost < minCost)
	                    || isNear(node.parent->splitCost, minCost)))
		return;

	point leftChildMaxBounds = node.maxBounds;
	leftChildMaxBounds[splitAxis] = minCostCoord;

	point rightChildMinBounds = node.minBounds;
	rightChildMinBounds[splitAxis] = minCostCoord;

	node.left = newNode(node.minBounds, leftChildMaxBounds);
	node.right = newNode(rightChildMinBounds, node.maxBounds);
	node.left->parent = node.right->parent = &node;
	node.left->next = node.right;

	trigRef = node.triangles;

	while (trigRef) {
		struct TrigRef *nextRef = trigRef->next;
		struct TrigRef *refClone = NULL;
		bool inLeftChild = false;
		bool inRightChild = false;

		for (int i = 0; i < 3; i++) {
			if (trigRef->triangle->vertices[i][splitAxis] < minCostCoord)
				inLeftChild = true;
			else
				inRightChild = true;
		}

		if (inLeftChild) {
			if (inRightChild)
				refClone = newRef(trigRef->triangle);

			trigRef->next = node.left->triangles;
			node.left->triangles = trigRef;
			node.left->nbTriangles++;
		}

		if (refClone) {
			refClone->next = node.right->triangles;
			node.right->triangles = refClone;
			node.right->nbTriangles++;
		} else if (inRightChild) {
			trigRef->next = node.right->triangles;
			node.right->triangles = trigRef;
			node.right->nbTriangles++;
		}

		trigRef = nextRef;
	}
	node.triangles = NULL;

	assert(node.left->nbTriangles > 0);
	assert(node.right->nbTriangles > 0);
	node.splitAxis = splitAxis;
	node.left->depth = node.right->depth = node.depth + 1;
	node.splitCost = minCost;
}

KdTree::KdTree(struct Scene &scene) {

	cout << "(building k-d tree... ";
	fflush(stdout);

	// Initializations
	nextFreeNodeIndex = 0;
	nextFreeRefIndex = 0;

	// Generate tree root
	root = newNode((point){-1.5, -1.5, -1.5}, (point){1.5, 1.5, 1.5});
	root->nbTriangles = scene.nbTriangles;

	for (unsigned int i = 0; i < scene.nbTriangles; i++) {
		struct Triangle *triangle = &scene.triangles[i];
		struct TrigRef *ref = newRef(triangle);

		ref->next = root->triangles;
		root->triangles = ref;
	}

	root->depth = 0;

#ifndef DISABLE_KDTREE
	struct KdTreeNode *node = root;
	struct KdTreeNode *lastNode = root;

	while (node && (node->depth < KDTREE_MAX_DEPTH)) {
		if (node->nbTriangles > 1) {
			splitKdTreeNode(*node);
			if (node->left) {
				lastNode->next = node->left;
				lastNode = node->right;
			}
		}

		node = node->next;
	}
#endif

	cout << "done.) ";
	fflush(stdout);

}

inline bool isExplicitlyWithin(const point &min, const point &p,
                               const point &max) {
	return ((min.x < p.x) && (p.x < max.x)
	        && (min.y < p.y) && (p.y < max.y)
	        && (min.z < p.z) && (p.z < max.z));
}

bool computeKdTreeNodeEntryPoint(struct KdTreeNode *node, struct Ray *ray,
                                point &entryPoint) {
	// If the start point is inside the node, it is the entry point
	if (isExplicitlyWithin(node->minBounds, ray->start, node->maxBounds)) {
		entryPoint = ray->start;
		return true;
	}

	for (int normalAxis = 0; normalAxis < 3; normalAxis++) {
		vector n = {0, 0, 0};
		point V0;
		scalar s;

		n[normalAxis] = 1;

		// Test if ray is parallel to the planes
		if (isNear(ray->direction * n, 0)) {
			entryPoint = ray->start;

			// The problem is reduced to 2 dimensions
			const int axis[2] = {(normalAxis + 1) % 3,
			                     (normalAxis + 2) % 3};
			for (int i = 0; i < 2; i++) {
				int otherAxis = axis[(i + 1) % 2];
				scalar s;

				// Test if ray is orthogonal to the axis
				if (isNear(ray->direction[axis[i]], 0)) {
					entryPoint = ray->start;

					// The problem is reduced to 1 dimension
					if (isNear(ray->direction[otherAxis], 0))
						// Never entries
						return false;
					else if (ray->direction[otherAxis] > 0)
						// Entry through the min point
						entryPoint[otherAxis] = node->minBounds[otherAxis];
					else if (ray->direction[otherAxis] < 0)
						// Entry through the max point
						entryPoint[otherAxis] = node->maxBounds[otherAxis];

					return true;
				} else if (ray->direction[axis[i]] > 0) {
					// The ray is coming in the same direction
					// as the axis. It will entry through the MIN line.
					s = ((node->minBounds[axis[i]] - ray->start[axis[i]])
					     / ray->direction[axis[i]]);
					entryPoint[axis[i]] = node->minBounds[axis[i]];
				} else if (ray->direction[axis[i]] < 0) {
					// The ray is coming in the opposite direction
					// of the axis. It will entry through the MAX line.
					s = ((node->maxBounds[axis[i]] - ray->start[axis[i]])
					     / ray->direction[axis[i]]);
					entryPoint[axis[i]] = node->maxBounds[axis[i]];
				}

				// Check if the intersection is valid
				if (s > 0) {
					scalar otherEntry = (ray->direction[otherAxis] * s
					                    + ray->start[otherAxis]);
					if (isWithin(node->minBounds[otherAxis],
					             otherEntry,
					             node->maxBounds[otherAxis])) {
						if (isNear(otherEntry, node->minBounds[otherAxis]))
							entryPoint[otherAxis] = node->minBounds[otherAxis];
						else if (isNear(otherEntry, node->maxBounds[otherAxis]))
							entryPoint[otherAxis] = node->maxBounds[otherAxis];
						else
							entryPoint[otherAxis] = otherEntry;

						return true;
					}
				}
			}

			// If we are here, then the ray never intersects the node.
			return false;
		} else if ((ray->direction * n) < 0) {
			// Ray is not parallel but is coming in the axis opposite
			// direction. Its entry point will be in the MAX plane.
			n[normalAxis] = -1;
			V0 = node->maxBounds;
		} else {
			// Ray is not parallel and is coming in the same direction
			// of the axis. Its entry point will be in the MIN plane.
			V0 = node->minBounds;
		}

		// Find the entry point.
		s = (n * (V0 - ray->start)) / (n * ray->direction);

		// Check if the intersection is valid
		if (s >= 0) {
			point intersection = ray->start + s * ray->direction;
			point &minBounds = node->minBounds;
			point &maxBounds = node->maxBounds;
			bool isValid = true;

			intersection[normalAxis] = V0[normalAxis];

			for (int i = 0; i < 3; i++) {
				if (i == normalAxis)
					continue;

				if (!isWithin(minBounds[i], intersection[i], maxBounds[i]))
					isValid = false;
				else {
					if (isNear(minBounds[i], intersection[i]))
						intersection[i] = minBounds[i];
					else if (isNear(maxBounds[i], intersection[i]))
						intersection[i] = maxBounds[i];
				}
			}

			if (isValid) {
				entryPoint = intersection;
				return true;
			}
		}
	}

	// If we are here, then the ray never intersects the node.
	return false;
}


bool computeKdTreeNodeExitPoint(struct KdTreeNode *node,
                                struct Ray *ray,
                                point &exitPoint) {
	for (int normalAxis = 0; normalAxis < 3; normalAxis++) {
		vector n = {0, 0, 0};
		point V0;
		scalar s;

		n[normalAxis] = 1;

		// Test if ray is parallel to the planes
		if (isNear(ray->direction * n, 0)) {
			exitPoint = ray->start;

			// The problem is reduced to 2 dimensions
			const int axis[2] = {(normalAxis + 1) % 3,
			                     (normalAxis + 2) % 3};
			for (int i = 0; i < 2; i++) {
				int otherAxis = axis[(i + 1) % 2];
				scalar s;

				// Test if ray is orthogonal to the axis
				if (isNear(ray->direction[axis[i]], 0)) {
					exitPoint = ray->start;

					// The problem is reduced to 1 dimension
					if (isNear(ray->direction[otherAxis], 0))
						// Never exits
						return false;
					else if (ray->direction[otherAxis] > 0)
						// Exit through the max point
						exitPoint[otherAxis] = node->maxBounds[otherAxis];
					else if (ray->direction[otherAxis] < 0)
						// Exit through the min point
						exitPoint[otherAxis] = node->minBounds[otherAxis];

					return true;
				} else if (ray->direction[axis[i]] > 0) {
					// The ray is coming in the same direction
					// as the axis. It will exit through the MAX line.
					s = ((node->maxBounds[axis[i]] - ray->start[axis[i]])
					     / ray->direction[axis[i]]);
					exitPoint[axis[i]] = node->maxBounds[axis[i]];
				} else if (ray->direction[axis[i]] < 0) {
					// The ray is coming in the opposite direction
					// of the axis. It will exit through the MIN line.
					s = ((node->minBounds[axis[i]] - ray->start[axis[i]])
					     / ray->direction[axis[i]]);
					exitPoint[axis[i]] = node->minBounds[axis[i]];
				}

				// Check if the intersection is valid
				if (s > 0) {
					scalar otherExit = (ray->direction[otherAxis] * s
					                    + ray->start[otherAxis]);
					if (isWithin(node->minBounds[otherAxis],
					             otherExit,
					             node->maxBounds[otherAxis])) {
						if (isNear(otherExit, node->minBounds[otherAxis]))
							exitPoint[otherAxis] = node->minBounds[otherAxis];
						else if (isNear(otherExit, node->maxBounds[otherAxis]))
							exitPoint[otherAxis] = node->maxBounds[otherAxis];
						else
							exitPoint[otherAxis] = otherExit;

						return true;
					}
				}
			}

			// If we are here, then the ray never intersects the node.
			return false;
		} else if ((ray->direction * n) < 0) {
			// Ray is not parallel but is coming in the axis opposite
			// direction. Its exit point will be in the MIN plane.
			n[normalAxis] = -1;
			V0 = node->minBounds;
		} else {
			// Ray is not parallel and is coming in the same direction
			// of the axis. Its exit point will be in the MAX plane.
			V0 = node->maxBounds;
		}

		// Find the exit point.
		s = (n * (V0 - ray->start)) / (n * ray->direction);

		// Check if the intersection is valid
		if (s >= 0) {
			point &minBounds = node->minBounds;
			point &maxBounds = node->maxBounds;
			bool isValid = true;
			exitPoint = ray->start + s * ray->direction;

			exitPoint[normalAxis] = V0[normalAxis];

			for (int i = 0; i < 3; i++) {
				if (i == normalAxis)
					continue;

				if (!isWithin(minBounds[i], exitPoint[i], maxBounds[i]))
					isValid = false;
				else {
					if (isNear(minBounds[i], exitPoint[i]))
						exitPoint[i] = minBounds[i];
					else if (isNear(maxBounds[i], exitPoint[i]))
						exitPoint[i] = maxBounds[i];
				}
			}

			if (isValid)
				return true;
		}
	}

	// If we are here, then the ray never intersects the node.
	return false;
}

struct KdTreeNode* KdTree::getNextNode(struct KdTreeNode *currentNode,
                                       struct Ray *ray) {
	struct KdTreeNode *node = currentNode;
	point exitPoint;

	if (!node) {

#ifdef DISABLE_KDTREE
		return root;
#endif

		node = root;
		if (!computeKdTreeNodeEntryPoint(node, ray, exitPoint))
		    return NULL;
	} else {

#ifdef DISABLE_KDTREE
		return NULL;
#endif

		if (!computeKdTreeNodeExitPoint(currentNode, ray, exitPoint))
			return NULL;
		while (!isExplicitlyWithin(node->minBounds, exitPoint,
		                           node->maxBounds)) {
			node = node->parent;
			if (!node)
				return NULL;
		}
	}

	while (!node->triangles) {
		const int splitAxis = node->splitAxis;
		const coord splitCoord = node->left->maxBounds[splitAxis];

		if (exitPoint[splitAxis] == splitCoord) {
			if (ray->direction[splitAxis] > 0) {
				node = node->right;
			} else {
				node = node->left;
			}
		} else if (exitPoint[splitAxis] > splitCoord) {
			node = node->right;
		} else {
			node = node->left;
		}
	}

	return node;
}
