#ifndef __SCENE_H
#define __SCENE_H

#include "DefTypes.h"

typedef struct _scene {
	//all Triangles
	triangle triangles [MAXTRIANGLES];
	//all lights
	light lights [MAXLIGHTS];
	//all materials
	material materials[MAXMATERIALS];
    //result image size
	int sizex, sizey;
	//total of triangles
    unsigned int NbTriangles;
    //total of lights
    unsigned int NbLights;
    //total of materials
    unsigned int NbMaterials;
} scene;

//struct that stocks possible intersection point
//I     - possible intersection point
//valid - it means if this point is inside the triangle packet or not
struct pairInt
{
	point_fixed I;
	bool valid;
};

//struct that stocks possible splitting point
//split pos - possible splitting point
//n1count   - number of triangles in the LEFT site
//n2count   - number of triangles in the RIGHT site
struct SplitList
{
	coord splitpos;
	unsigned int n1count, n2count;
	SplitList* next;
};

//struct that stores all scene elements
extern scene myScene;

bool init( char* sceneDescName, char* trigFileName, scene* myScene );
int GetTriangle ( char* trigFileName, unsigned int* NbTriangles, triangle triangles[MAXTRIANGLES] );
ifixed disq( kdTreeNode* ctr, point_fixed pos);
bool build( TrigRefList** trianglelist, TrigRefList** freeRefNodes, kdTreeNode** freeNodes, kdTreeNode * tree, const int depth );
kdTreeNode * traverse(kdTreeNode * const ctr, const point_fixed  pos);
int getKdNodeIntersections( ray_fixed* ray, kdTreeNode* i, pairInt intersections[6] );
kdTreeNode* getTrianguleList( ray_fixed* ray, kdTreeNode* curNode, kdTreeNode* tree );

extern void deliverNode( kdTreeNode* node, kdTreeNode** freeNodes );
extern void deliverNode( TrigRefList* node, TrigRefList** freeNodes );

extern kdTreeNode* requestNode( kdTreeNode** freeNodes );
extern TrigRefList* requestNode( TrigRefList** freeNodes );

extern kdTreeNode* newNode( kdTreeNode** freeNodes, const coord X0, const coord Y0, const coord Z0, const coord X1, const coord Y1, const coord Z1 );

#endif // __SCENE_H
