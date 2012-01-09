#ifndef RAYTRACER_H_
#define RAYTRACER_H_
#include "DefTypes.h"
#include "Scene.h"

//all triangle references to be put in
extern TrigRefList refTrigs[ 2*MAXTRIANGLES ];

//All kdTreeNodes
extern kdTreeNode nodes[ MAXNODES ];

void initFreeNodeList( kdTreeNode nodes[MAXNODES], kdTreeNode* freeNodes );
TrigRefList* initFreeNodeList( TrigRefList nodes[MAXTRIGREFS] );
kdTreeNode* initFreeNodeList( kdTreeNode nodes[MAXNODES] );
node* initFreeNodeList( node rayNodes[MAXRAYS] );

void deliverNode( kdTreeNode* node, kdTreeNode** freeNodes );
void deliverNode( TrigRefList* node, TrigRefList** freeNodes );

kdTreeNode* requestNode( kdTreeNode** freeNodes );
TrigRefList* requestNode( TrigRefList** freeNodes );

kdTreeNode* newNode( kdTreeNode** freeNodes, const coord X0, const coord Y0, const coord Z0, const coord X1, const coord Y1, const coord Z1 );

bool draw(char* outputName, scene &myScene);

#endif /* RAYTRACER_H_ */
