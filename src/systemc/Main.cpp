#include "RayTracer.h"

int main(int argc, char* argv[])
{
	//pointer for the first kdTree free Node
	kdTreeNode* freeNodes;
	//pointers for triangle reference management
	TrigRefList * freeRefs, *newRef, *trigRefs;

	//initialisating kdTreeFreeNode list
	freeNodes = initFreeNodeList( nodes );
	freeRefs = initFreeNodeList( refTrigs );

	//cout << "square root adventure" << endl;
	//cout << "level 1 - 4 " << square_root(4) << endl;
	//cout << "level 2 - 24.69 " << square_root(24.69) << endl;

	if (argc < 4)
	{
		cout << "Usage : Raytrace.exe scene.dsc scene.off output.tga" << endl;
		return -1;
	}

	if ( !init( argv[1], argv[2], &myScene ) )
	{
		cout << "Failure when reading the Scene file." << endl;
		return -1;
	}
	cout << "initialised" << endl;

	//Initializing kdTree
	myScene.tree = newNode( &freeNodes, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0 );
	myScene.tree->pnum = myScene.NbTriangles;

	if( myScene.NbTriangles > 0 )
	{
		newRef = requestNode( &freeRefs );
		newRef->ref = myScene.triangles;
		newRef->next = NULL;
		trigRefs = newRef;
	}

	for( unsigned int i = 1; i < myScene.NbTriangles; i++ )
	{
		newRef = requestNode ( &freeRefs );
		newRef->ref = &myScene.triangles[i];
		newRef->next = trigRefs;
		trigRefs = newRef;
	}

	myScene.tree->trig = trigRefs;

	/*while ( trigRefs != NULL )
    {
    	cout << "Index: " << trigRefs->ref->index << endl;
    	cout << "Small: " << trigRefs->ref->small.x << " " << trigRefs->ref->small.y << " " << trigRefs->ref->small.z << endl;

    	trigRefs = trigRefs->next;
    }

    for ( int i = 0; i < tree->pnum; i++ )
    {
      	cout << "Index: " << myScene.triangles[i].index << endl;
      	cout << "Small: " << myScene.triangles[i].small.x << " " << myScene.triangles[i].small.y << " " << myScene.triangles[i].small.z << endl;
    }

    trigRefs = tree->trig;
	 */

	if( !build( &trigRefs, &freeRefs, &freeNodes, myScene.tree, KDTREEDEPTH ) )
	{
		printf("error: no memory\n");
		return -1;
	}

	printf("drawing\n");

	if ( !draw(argv[3], &myScene ) )
	{
		cout << "Failure when creating the image file." << endl;
		return -1;
	}
	return 0;
}
