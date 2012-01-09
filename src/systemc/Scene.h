#ifndef __SCENE_H
#define __SCENE_H

#include "DefTypes.h"

const vect_fixed NullVector = { 0,0,0 };
const point_fixed Origin = { 0,0,0 };

//this variavles are all inside raytracer.h
extern SplitList splits[ MAXTRIGREFS ];
extern triangle *tleft[ MAXTRIGREFS ];
extern triangle *tright[ MAXTRIGREFS ];
extern point_fixed vertex[MAXPOINTS];

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
    //reference for the scene kdTree
    kdTreeNode * tree;
} scene;

ifixed disq( kdTreeNode* ctr, point_fixed pos);

void insertSplitPos( SplitList** sList, SplitList** splitPool, coord splitPos );

bool insersectBox( ifixed limits[6], triangle* t );

bool build( TrigRefList** trianglelist, TrigRefList** freeRefNodes, kdTreeNode** freeNodes, kdTreeNode * tree, const int depth );

kdTreeNode * traverse(kdTreeNode * ctr, const point_fixed  pos);

int getKdNodeIntersections( ray* ray, kdTreeNode* i, pairInt intersections[6] );

kdTreeNode* getTrianguleList( ray* ray, kdTreeNode* curNode, kdTreeNode* tree );

int getTriangle ( char* trigFileName, unsigned int* NbTriangles, triangle triangles[MAXTRIANGLES] );

bool init( char* sceneDescName, char* trigFileName, scene* myScene );

#endif // __SCENE_H
