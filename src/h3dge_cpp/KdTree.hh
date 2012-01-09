#ifndef _KDTREE_HH_
#define _KDTREE_HH_

#include "Scene.hh"
#include "NumTypes.hh"
#include "Ray.hh"

#define KDTREE_MAX_DEPTH 16
#define KDTREE_MAX_NODES ((1 << (KDTREE_MAX_DEPTH + 1)) - 1)
#define KDTREE_MAX_TRIG_REFS (4 * SCENE_MAX_TRIANGLES)
//#define DISABLE_KDTREE

struct TrigRef {
	struct Triangle *triangle;
	struct TrigRef  *next;
};

struct KdTreeNode {
	struct KdTreeNode *left;
	struct KdTreeNode *right;
	struct KdTreeNode *parent;
	struct KdTreeNode *next;

	struct TrigRef *triangles;
	uint32_t nbTriangles;

	point minBounds;
	point maxBounds;

	uint32_t splitAxis;
	uint32_t depth;
	scalar splitCost;
};

inline bool isWithin(const point &p, const struct KdTreeNode &node) {
	return (isWithin(node.minBounds.x, p.x, node.maxBounds.x)
	        && isWithin(node.minBounds.y, p.y, node.maxBounds.y)
	        && isWithin(node.minBounds.z, p.z, node.maxBounds.z));
}

class KdTree {
private:
	struct KdTreeNode NodePool[KDTREE_MAX_NODES];
	unsigned int nextFreeNodeIndex;
	struct KdTreeNode *newNode(const point &minBounds, const point &maxBounds);

	struct TrigRef RefPool[KDTREE_MAX_TRIG_REFS];
	unsigned int nextFreeRefIndex;
	struct TrigRef *newRef(struct Triangle *triangle);

	void splitKdTreeNode(struct KdTreeNode &node);

public:
	struct KdTreeNode *root;

	struct KdTreeNode* getNextNode(struct KdTreeNode *currentNode,
	                               struct Ray *ray);

	explicit KdTree(struct Scene &scene);
};

#endif
