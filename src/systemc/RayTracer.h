#ifndef RAYTRACER_H_
#define RAYTRACER_H_
#include "DefTypes.h"
#include "Scene.h"

//struct that stocks all scene elements
extern scene myScene;

//stocks splitting point canditates
extern SplitList splits[ MAXTRIGREFS ];

//each element stocks an address of a triangle. This structure is used inside
//KDTree for not dealing with the triangles but only with its references in
//the tree building process
extern TrigRefList refTrigs[ 2*MAXTRIANGLES ];

//the nodes of the kdTree. The building process will link them properly
extern kdTreeNode nodes[ MAXNODES ];

//stocks the triangle references whose axis coordinate is smaller than the splitting point
extern triangle *tleft[ MAXTRIGREFS ];

//stocks the triangle references whose axis coordinate is bigger than the splitting point
extern triangle *tright[ MAXTRIGREFS ];

//used for stocking the points of triangles
//TODO: this is only a structure that is used for charging the triangles
//So, in the definitive implementation, It is necessary destroy-it after charging the triangles
//Due a stack_overflow_error, this vector is not inside GetTriangle
extern point_fixed vertex[MAXPOINTS];


int hitTriangle ( ray *r, const triangle *Tr, coord *t );

triangle* figureIntersection ( node* currNode, scene *myScene, coord *t );

int isReflected( node* currNode, scene *myScene, triangle** intersection, coord *param_t );

int isRefracted( node* currNode, scene *myScene, triangle** intersection, coord *param_t );

vect_fixed normalTriangle( triangle* current, scene *myScene );

ray createReflected ( ray r, scene* myScene, triangle* current, coord param_t );

ray createRefracted ( ray r, scene *myScene, int state, int *type, int *state1, triangle* current, coord param_t );

node *treeGenerator(node** freeRayNodes, int hmax, node *root, scene *myScene);

color_fixed lightContribution ( node * currNode, scene *myScene );

color_fixed raytracer (ray *r, kdTreeNode* currentPacket, scene *myScene );

bool draw( char* outputName, scene *myScene );

#endif /* RAYTRACER_H_ */
