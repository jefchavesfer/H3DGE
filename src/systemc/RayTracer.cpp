#include "RayTracer.h"

scene myScene;

//stocks splitting point canditates
SplitList splits[ MAXTRIGREFS ];

//each element stocks an address of a triangle. This structure is used inside
//KDTree for not dealing with the triangles but only with its references in
//the tree building process
TrigRefList refTrigs[ 2*MAXTRIANGLES ];

//the nodes of the kdTree. The building process will link them properly
kdTreeNode nodes[ MAXNODES ];

//stocks the triangle references whose axis coordinate is
//smaller than the splitting point
triangle *tleft[ MAXTRIGREFS ];

//stocks the triangle references whose axis coordinate is
//bigger than the splitting point
triangle *tright[ MAXTRIGREFS ];

//used for stocking the points of triangles
//TODO: this is only a structure that is used for charging the triangles
//So, in the definitive implementation, It is necessary destroy-it after
//charging the triangles
//Due a stack_overflow_error, this vector is not inside GetTriangle
point_fixed vertex[MAXPOINTS];


 // This function determines whether a point is inside a triangle,
 //using barycenter coordinates
int hitTriangle ( ray *r, const triangle *Tr, coord *t )
{
	vect_fixed e1 = { Tr->V2.x - Tr->V1.x,
	                  Tr->V2.y - Tr->V1.y,
	                  Tr->V2.z - Tr->V1.z }; //E1
	vect_fixed e2 = { Tr->V3.x - Tr->V1.x,
	                  Tr->V3.y - Tr->V1.y,
	                  Tr->V3.z - Tr->V1.z }; //E2
	vect_fixed t1 = { r->start.x - Tr->V1.x,
	                  r->start.y - Tr->V1.y,
	                  r->start.z - Tr->V1.z};//T
	vect_fixed P = { ( r->dir.y ) * ( e2.z ) - ( r->dir.z ) * ( e2.y ),
	                 ( e2.x ) * ( r->dir.z ) - ( r->dir.x ) * ( e2.z ),
	                 ( r->dir.x ) * ( e2.y ) - ( e2.x ) * ( r->dir.y ) };
	vect_fixed Q = { ( t1.y ) * ( e1.z ) - ( t1.z ) * ( e1.y ),
	                 ( e1.x ) * ( t1.z ) - ( t1.x ) * ( e1.z ),
	                 ( t1.x ) * ( e1.y ) - ( e1.x ) * ( t1.y ) };

	coord det = P * e1;


	if ( det > - 0.001 && det < 0.001 )
	{
		return 0;
	}


	coord u = (coord)( ( t1 * P )/det);


	if ( u < 0 || (u > 1.0001) )
	{
		return 0;
	}
	coord v = ( coord )( ( r->dir * Q )/det);


	if ( (v < 0) || ( (u + v) > 1.0001) )
	{
		return 0;
	}

	*t =( coord )( ( e2 * Q ) / det);// Q * e2 * indet;

	if( *t >= 0.001 && *t <= 2)
	{

		return 1;
	}
	else
	{
		return 0;
	}
}

//Loop for going over the set of triangles
triangle* figureIntersection ( node* currNode, scene *myScene, coord* t )
{
	int intersect = 0; //flag for intersection occurrence
	coord limit = 2;  //space max bound
	triangle* current = NULL;
	kdTreeNode* currPacket = currNode->currentPacket;
	coord u; //multiplication factor. The closest triangle has the smallest u
	//printf("intersection verification process\n");
	while( 1 )
	{
		TrigRefList* currTrigRef = currNode->currentPacket->trig;
		while( currTrigRef != NULL )
		{
			if ( hitTriangle( &( currNode->v ), currTrigRef->ref, &u ) == 1 )
			{
				//avoids self intersection
				if( currTrigRef->ref != currNode->lastIntercepted )
				{
					if ( u < limit )
					{
						current = currTrigRef->ref;
						limit = u;
						intersect = 1;
						//cout << "intersect! " << currTrigRef->ref->index
						//     << endl;
					}
				}
			}
			// cout << "u: " << u << " limit " << limit
			//      << " index " << currTrigRef->ref->index << endl;
			currTrigRef = currTrigRef->next;
		}
		if( intersect == 1 )
		{
			//this avoids self intersection
			currNode->lastIntercepted = current;
			// printf( "Intersection found - Trig Index: %d\n\n",
			//         current->index );
			// cout << "current V1: x" <<  current->V1.x
			//      << " y " << current->V1.y
			//      << " z " <<  current->V1.z << endl;
			// cout << "current V1: x" <<  current->V2.x
			//      << " y " << current->V2.y
			//      << " z " <<  current->V2.z << endl;
			// cout << "current V1: x" <<  current->V3.x
			//      << " y " << current->V3.y
			//      << " z " <<  current->V3.z << endl;
			// cout << "packet: x0 " << currNode->currentPacket->x0
			//      << " y0 " << currNode->currentPacket->y0
			//      << " z0 " << currNode->currentPacket->z0 << endl;
			// cout << "packet: x1 " << currNode->currentPacket->x1
			//      << " y1 " << currNode->currentPacket->y1
			//      << " z0 " << currNode->currentPacket->z1 << endl;
			// exit(0);
			break; //intersection found
		}
		else
		{
			//printf( "getting next packet\n" );
			currPacket = getTrianguleList( &( currNode->v ) ,
			                               currNode->currentPacket,
			                               myScene->tree );
			if( currPacket != NULL && currPacket != currNode->currentPacket )
			{
				// currTrigRef = currPacket->trig;
				// for( int i = 0; i < currPacket->pnum; i++ )
				// {
				// 	cout << "index: " << currTrigRef->ref->index << endl;
				// 	cout << "v1  - x: " << currTrigRef->ref->V1.x
				// 	     << " y: " << currTrigRef->ref->V1.y
				// 	     << " z: " << currTrigRef->ref->V1.z << endl;
				// 	cout << "v2  - x: " << currTrigRef->ref->V2.x
				// 	     << " y: " << currTrigRef->ref->V2.y
				// 	     << " z: " << currTrigRef->ref->V2.z << endl;
				// 	cout << "v3  - x: " << currTrigRef->ref->V3.x
				// 	     << " y: " << currTrigRef->ref->V3.y
				// 	     << " z: " << currTrigRef->ref->V3.z << endl;
				// 	cout << endl;
				// 	currTrigRef = currTrigRef->next;
				// }
			}
			else
			{
				//printf( "no more triangles\n" );
				break;
			}
			currNode->currentPacket = currPacket;
		}
	}

	if ( current != NULL )
	{
		*t = limit;
	}

	return current;
}

//Function to determine if the surface can have a reflected ray
int isReflected(node* currNode, scene *myScene,
                triangle** intersection, coord *param_t)
{
	coord t = currNode->t;
	triangle* current = currNode->lastIntercepted;
	if ( current == NULL )
	{
		return 0;
	}
	material cMat = myScene->materials[ current->materialId ];

	if ( cMat.reflection == 0 )
		return 0;
	else
	{
		*intersection = current;
		*param_t = t;

		return 1;
	}
}

//Function to determine if the surface can have a refracted ray
int isRefracted( node* currNode, scene *myScene,
                 triangle** intersection, coord *param_t )
{
	coord t = currNode->t;
	triangle* current = currNode->lastIntercepted;

	if ( current == NULL )
		return 0;
	material cMat = myScene->materials[ current->materialId ];

	if ( cMat.transmission == 0 && cMat.nrefraction <= 1 )
		return 0;
	else
	{
		*intersection = current;
		*param_t = t;
		return 1;
	}
}

//Function to calculate the normal vector of a triangle
vect_fixed normalTriangle( triangle* current, scene *myScene )
{
	vect_fixed v1 = { current->V2.x - current->V1.x,
	                  current->V2.y - current->V1.y,
	                  current->V2.z - current->V1.z };
	vect_fixed v2 = { current->V2.x - current->V3.x,
	                  current->V3.y - current->V1.y,
	                  current->V3.z - current->V1.z };
	vect_fixed normal={(v1.y)*(v2.z)-(v1.z)*(v2.y),
	                   (v2.x)*(v1.z)-(v1.x)*(v2.z),
	                   (v1.x)*(v2.y)-(v2.x)*(v1.y)};
	coord temp = square_root(normal*normal);
	sc_dt::sc_fixed< 32, 16, sc_dt::SC_TRN, sc_dt::SC_WRAP > temp1  = 1 / temp;
	normal.x=normal.x*temp1;
	normal.y=normal.y*temp1;
	normal.z=normal.z*temp1;
	return normal;
}

//Function to create a reflected ray
ray createReflected ( ray r, scene* myScene, triangle* current, coord param_t )
{
	ray RR = { { 0, 0, 0 }, { 0, 0, 0 } };
	point_fixed IntersectionPoint = r.start + param_t * r.dir;
	vect_fixed normal = normalTriangle( current, myScene );

	if (r.dir * normal > 0)
	{
		normal.x=-normal.x;
		normal.y=-normal.y;
		normal.z=-normal.z;
	}

	RR.start = IntersectionPoint;
	RR.dir = r.dir- 2 * ( normal * r.dir ) * normal;
	return RR;
}

//Function to create a refracted ray
ray createRefracted ( ray r, scene *myScene, int state, int *type,
                      int *state1, triangle* current, coord param_t )
{

	ray RR={ { 0, 0, 0 }, { 0, 0, 0 } };
	int type1 = 0;
	int state2 = 0;
	point_fixed intersectionPoint = r.start + param_t * r.dir;
	vect_fixed normal = normalTriangle( current, myScene );

	if (r.dir * normal > 0)
	{
		normal.x = -normal.x;
		normal.y = -normal.y;
		normal.z = -normal.z;
	}

	RR.start = intersectionPoint;
	mfp n = myScene->materials[ current->materialId ].nrefraction;

	if ( state == INSIDE )
	{
		n = (mfp)( n / 1.003 );
	}
	else
	{
		n = (mfp)( 1.003 / n );
	}

	coord total_reflection = 1 - n * n * ( 1 - ( r.dir * normal )
	                                       * ( r.dir * normal ) );
	if( total_reflection <= 1 )
	{
		RR.dir = n * r.dir - ( n * r.dir * normal
		                       + square_root( total_reflection ) ) * normal;
		type1 = REFRACTED_RAY;
		if ( state == INSIDE )
		{
			state2 = OUTSIDE;
		}
		else
		{
			state2 = INSIDE;
		}
	}
	else
	{
		RR.dir = r.dir - 2 * ( normal * r.dir ) * normal;
		type1 = TOTAL_REFLECTED_RAY;
		state2 = INSIDE;
	}
	*type = type1;
	*state1 = state2;
	return RR;
}

//Tree for the secondary rays
node* treeGenerator( node** freeRayNodes, int hmax,
                     node *currNode, scene *myScene ) {
	node * result;
	int type1 = REFRACTED_RAY;
	int state1 = INSIDE;
	coord param_t = 0;
	triangle* current_fig = NULL;

	if ( currNode->level < hmax )
	{
		//printf("is reflect verification\n");
		if ( isReflected( currNode, myScene, &current_fig, &param_t ) == 1
		     && ( currNode->state == OUTSIDE || currNode->state == ROOT_RAY ) )
			// On est reste ici
		{
			result = requestNode( freeRayNodes );
			if( result == NULL )
			{
				return result;
			}
			// void initNode( node* curNode, node* parent, kdTreeNode* packet,
			//                ray r, triangle* trigIntersected, int h,
			//                int type, int state)
			initNode( result,
			          currNode,
			          currNode->currentPacket,
			          createReflected( currNode->v,
			                           myScene,
			                           current_fig,
			                           param_t ),
			          currNode->lastIntercepted,
			          currNode->level + 1,
			          REFLECTED_RAY ,
			          OUTSIDE );
			currNode->left = result;
			return result;
		}
		//printf("is refracted verification\n");
		if ( isRefracted( currNode, myScene, &current_fig, &param_t ) == 1 )
		{
			result = requestNode( freeRayNodes );
			if( result == NULL )
			{
				return result;
			}
			ray r_rf = createRefracted( currNode->v,
			                            myScene,
			                            currNode->state,
			                            &type1,
			                            &state1,
			                            current_fig,
			                            param_t );
			initNode( result,
			          currNode,
			          currNode->currentPacket,
			          r_rf,
			          currNode->lastIntercepted,
			          currNode->level + 1,
			          type1,
			          state1 );
			currNode->right = result;
			return result;
		}
	}

	node * current = currNode->parent;

	while ( current != 0 )
	{
		//printf("is refracted verification - returning\n");
		if( isRefracted( currNode, myScene, &current_fig, &param_t ) == 1 )
			break;
		current = current->parent;
	}

	if ( current == 0 ) {
		return 0;
	}

	result = requestNode( freeRayNodes );
	if ( result != NULL )
	{
		ray r_rf = createRefracted( current->v,
		                            myScene,
		                            current->state,
		                            &type1,
		                            &state1,
		                            current_fig,
		                            param_t );
		initNode( result,
		          current,
		          current->currentPacket,
		          r_rf,
		          current->lastIntercepted,
		          current->level + 1,
		          type1,
		          state1 );
		current->right = result;
	}
	else
	{
		printf(" no node available\n");
	}
	return result;
}

//Function to calculate the color contribution of each ray
color_fixed lightContribution ( node* currNode, scene *myScene )
{
	color_fixed c1 = { 0, 0, 0 };
	coord t = currNode->t;
	triangle* intersection = currNode->lastIntercepted;
	if ( intersection == NULL )
	{
		//printf( "no intersection \n" );
		return c1;
	}

	point_fixed intersectionPoint = currNode->v.start + t * currNode->v.dir;
	vect_fixed normal = normalTriangle( intersection, myScene );
	material intersectMat = myScene->materials[ intersection->materialId ];

	for( unsigned int j = 0; j < myScene->NbLights; ++j )
	{
		coord n1, n2;
		light currentl = myScene->lights[j];
		// vector length from the intersection point to the l
		vect_fixed dist = currentl.pos - intersectionPoint;
		n1 = currNode->v.dir * normal;
		//root->v.dir.x * normal.x + root->v.dir.y * normal.y + root->v.dir.z
		// * normal.z ;
		if (n1 > 0)
		{
			normal.x = -normal.x;
			normal.y = -normal.y;
			normal.z = -normal.z;
		}
		n1 = currNode->v.dir * normal;
		n2 = dist * normal;
		if( n1 < 0 && n2 > 0 )
		{
			//now let's create a ray from the intersection point to
			//the light source
			ray lightRay;
			lightRay.start = intersectionPoint;
			lightRay.dir = ( 1 / square_root( dist * dist ) ) * dist;
			//let's see if there are objects in the middle of the road that can
			// cause reflection or shadow
			// light ray and ray have to be in the same direction so proyection
			// from the light ray in the ray has to have the
			// same direction so cosI positive

			int shadow = 0;
			kdTreeNode* shadowPack = currNode->currentPacket;
			kdTreeNode* newShadowPack = NULL;
			//incrementing the ray starting point for avoiding self intersection

			//printf( "shadow verification process\n" );
			while( 1 )
			{
				TrigRefList* currTrigRef = shadowPack->trig;
				while( currTrigRef != NULL )
				{
					coord u = 0;
					if ( hitTriangle( &lightRay, currTrigRef->ref, &u ) == 1 )
					{
						if( u == 0 )
						{
							//not self intersection in the same packet
							currTrigRef = currTrigRef->next;
							continue;
						}
						if ( currTrigRef->ref != currNode->lastIntercepted )
						{
							//not self intersection in different packets
							//is it really necessary ?
							printf( "Intersection found - Trig Index: %d" \
							        "- shadow region for light %d\n\n ",
							        currTrigRef->ref->index, j );
							cout << "shadow ray start x: " << lightRay.start.x
							     << " y: " << lightRay.start.y
							     << " z: "  << lightRay.start.z << endl;
							cout << "shadow ray dir x  : " << lightRay.dir.x
							     << " y: " << lightRay.dir.y
							     << " z: "  << lightRay.dir.z   << endl;
							cout << "u: " << u;
							shadow = 1;
							break;
						}
						else
						{
							printf( "yes! It is necessary\n " );
							//exit(0);
						}
					}
					currTrigRef = currTrigRef->next;
				}
				if( shadow == 1 )
				{
					break; //shadow found
				}
				else
				{
					//printf( "getting next packet\n" );
					newShadowPack = getTrianguleList( &lightRay,
					                                  shadowPack,
					                                  myScene->tree );
					if( newShadowPack != NULL && newShadowPack != shadowPack )
					{
						currTrigRef = newShadowPack->trig;
						// for( int i = 0; i < newShadowPack->pnum; i++ )
						// {
						// 	cout << "index: " << currTrigRef->ref->index
						// 	     << endl;
						// 	cout << "v1  - x: " << currTrigRef->ref->V1.x
						// 	     << " y: " << currTrigRef->ref->V1.y << " z: "
						// 	     << currTrigRef->ref->V1.z << endl;
						// 	cout << "v2  - x: " << currTrigRef->ref->V2.x
						// 	     << " y: " << currTrigRef->ref->V2.y
						// 	     << " z: " << currTrigRef->ref->V2.z << endl;
						// 	cout << "v3  - x: " << currTrigRef->ref->V3.x
						// 	     << " y: " << currTrigRef->ref->V3.y
						// 	     << " z: " << currTrigRef->ref->V3.z << endl;
						// 	cout << endl;
						// 	currTrigRef = currTrigRef->next;
						// }
					}
					else
					{
						//cout << "no more triangles" << endl;
						break;
					}
					shadowPack = newShadowPack;
				}
			}
			//we considered the objects as lambert-surface ones so this means
			// that the Intensity from the incoming ray
			// depends on the angle between the normal

			//if there's no shadow, we considered the current light source
			// effect : Io=In*cos(0);
			if ( shadow == 0 )
			{

		    	mfp coef=1;
				for(unsigned int q=0; q<= (unsigned int)(currNode->level); ++q)
				{
					coef*=intersectMat.reflection;
				}

				if ((currNode->type==REFLECTED_RAY)
				    || (currNode->type==ROOT_RAY)
				    || (currNode->type==TOTAL_REFLECTED_RAY))
				{

					// lambert
					// cout << " coef:  " << coef
					//      << " level: " << root->level << endl;
					// intensity from objects material
					mfp lambert = (lightRay.dir * normal) * coef;
				   //  cout << " lambert:  " << lambert  << endl;
					c1.red = c1.red + lambert * currentl.red * intersectMat.red;
					c1.green = (c1.green + lambert * currentl.green
					            * intersectMat.green);
					c1.blue = (c1.blue + lambert * currentl.blue
					           * intersectMat.blue);

					// cout << "color r2 R " << c1.red
					//      << " g " << c1.green
					//      << " b " << c1.blue << endl;
				}

				else if (currNode->type==REFRACTED_RAY)
				{

					//pseudo beer
					c1.red = c1.red + coef * intersectMat.red;
					c1.green = c1.green + coef * intersectMat.green;
					c1.blue = c1.blue + coef * intersectMat.blue;
				}

			}
		}
		else
		{
			//cout << "FFFUUUU" << endl;
		}
	}
	return c1;
}


color_fixed raytracer ( ray *r, kdTreeNode* currentPacket, scene *myScene )
{
	// Initialing variables
	color_fixed c1 = {0,0,0}; //where to keep the color values
	node rayNodes[MAXRAYS];
	node* freeRayNodes;
	freeRayNodes = initFreeNodeList( rayNodes );

	//GENERATE TREE

	node* root = requestNode( &freeRayNodes );
	if ( root != NULL )
	{
		//cout << "root full" << endl;
		initNode( root, NULL, currentPacket, *r, NULL, 0, ROOT_RAY, OUTSIDE );
		node * courant = root;

		//adding and inserting nodes
		while ( courant != NULL )
		{
			// cout<< "ray: x " << courant->v.start.x
			//     << " y"<< courant->v.start.y
			//     <<" z " << courant->v.start.z <<endl;
			// cout << " Not arrived to the root yet" << endl;
			// cout << "ray start x"<< r.start.x
			//      << " y " << r.start.y
			//      << " z " << r.start.z <<endl;
			// cout << "ray dir x"<< r.dir.x
			//      << " y " << r.dir.y
			//      << " z " << r.dir.z <<endl;

			// cout << "r start x"<< courant->v.start.x
			//      << " y " << courant->v.start.y
			//      << " z " << courant->v.start.z <<endl;
			// cout << "r dir x"<< courant->v.dir.x
			//      << " y " << courant->v.dir.y
			//      << " z " << courant->v.dir.z <<endl;

			// cout << "intersection ray" << endl;
			courant->lastIntercepted = figureIntersection( courant,
			                                               myScene,
			                                               &courant->t );
			c1 += lightContribution ( courant, myScene );
			//cout << "intersection surface" << endl;
			courant = treeGenerator( &freeRayNodes,
			                         MAXDEPTH,
			                         courant,
			                         myScene );
		}
	}
	else
	{
		//cout << "no node available" << endl;
	}
	//the tree will be destroyed when exiting the function
	return c1;
}

bool draw( char* outputName, scene *myScene )
{
	ofstream imageFile(outputName,ios_base::binary);
	if (!imageFile)
		return false;
	// Addition of the TGA header
	imageFile.put(0).put(0);
	imageFile.put(2);        // RGB not compressed

	imageFile.put(0).put(0);
	imageFile.put(0).put(0);
	imageFile.put(0);

	imageFile.put(0).put(0); // origin X
	imageFile.put(0).put(0); // origin Y

	imageFile.put((unsigned char)(myScene->sizex & 0x00FF)).put(
		(unsigned char)((myScene->sizex & 0xFF00) / 256));
	imageFile.put((unsigned char)(myScene->sizey & 0x00FF)).put(
		(unsigned char)((myScene->sizey & 0xFF00) / 256));
	imageFile.put(24);       // 24 bit bitmap
	imageFile.put(0);
	// end of the TGA header

	pairInt intersections[6];
	TrigRefList* tref;
	kdTreeNode* currentPacket;

	// Scanning
	for (integer y = 0; y < myScene->sizey; ++y)
	{
		for (integer x = 0; x < myScene->sizex; ++x)
		{
			cout << "treating: ( " << x << " , " << y << " )" << endl;
			//float red = 0, green = 0, blue = 0;
			//float coef = 1.0f;
			//int level = 0;
			color_fixed c1={0,0,0};
			// Cast the ray
			// Because we are not in conic perspective (point of origin) but
			// in orthographic perspective, there is no natural starting point
			// We have to put it far enough to enclose the whole scene
			// but not too far to avoid floating point precision problems
			// (acne and other)
			// 1000.0f seems like a good compromise for now..
			ray viewRay = { {(coord)( x / ( ( integer ) LIMIT_SCALE ) ),
			                 (coord)( y / ( ( integer ) LIMIT_SCALE ) ),
			                 (coord)(  -1000 / ( ( integer ) LIMIT_SCALE )  ) },
			                {0,0,1} };

			point_fixed searchPoint;
			//cout << "getting first intersection" << endl;
			getKdNodeIntersections( &viewRay, myScene->tree, intersections );
			{
				int i, j;
				for( j = 0; j < 6; j ++ )
				{
					if( intersections[j].valid == true )
					{
						break;
					}
				}
				for( i = j + 1; i < 6; i++ )
				{
					if( intersections[i].valid == true )
					{
						break;
					}
				}
				if( disq( myScene->tree, intersections[i].I )
				    < disq( myScene->tree, intersections[j].I ) )
				{
					searchPoint = intersections[i].I;
				}
				else
				{
					searchPoint = intersections[j].I;
				}
			}
			// cout << "search Point - closer point - x: "
			//      << searchPoint.x << " y: " << searchPoint.y
			//      << " z: " << searchPoint.z << endl;

			//finding the first packet
			currentPacket = traverse( myScene->tree, searchPoint );
			//putting the ray start point inside the packet
			viewRay.start = searchPoint;

			// cout << "first triangle packet: " << endl;
			// tref = currentPacket->trig;
			// for( int k = 0; k < currentPacket->pnum; k++ )
			// {
			// 	cout << "index: " << tref->ref->index << endl;
			// 	cout << "v1  - x: " << tref->ref->V1.x << " y: "
			// 	     << tref->ref->V1.y << " z: " << tref->ref->V1.z << endl;
			// 	cout << "v2  - x: " << tref->ref->V2.x << " y: "
			// 	     << tref->ref->V2.y << " z: " << tref->ref->V2.z << endl;
			// 	cout << "v3  - x: " << tref->ref->V3.x << " y: "
			// 	     << tref->ref->V3.y << " z: " << tref->ref->V3.z << endl;
			// 	cout << endl;
			// 	tref = tref->next;
			// }

			c1 = raytracer( &viewRay, currentPacket, myScene );
			// cout << " c1 R " << c1.red *255
			//      << " G " << c1.green*255
			//      << " B " << c1.blue*255 << endl;
			imageFile.put((unsigned char)min(float(c1.blue)*255.0f,255.0f)).put(
				(unsigned char)min(float(c1.green)*255.0f, 255.0f)).put(
					(unsigned char)min(float(c1.red)*255.0f, 255.0f));
		}
	}
	return true;
}
