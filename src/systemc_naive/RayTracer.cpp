#include "RayTracer.h"
#include "Scene.h"


//function for generating a pool of rays

node* initFreeNodeList( node rayNodes[MAXRAYS] )
 {
	 int i;
	 node* freeNodes;
	 rayNodes[0].next = NULL;

	 for( i = 1; i < MAXRAYS; i++ )
	 {
		 rayNodes[i].next = &rayNodes[i-1];
		 //cout << "ni.next " << nodes[i].next << endl;
	 }
	 freeNodes = & ( rayNodes[MAXRAYS - 1] );
	 //cout << "last " << freeNodes << endl;
	 return freeNodes;
 }

void deliverNode( node* rayNode, node** freeRayNodes )
{
	rayNode->next = *freeRayNodes;
	*freeRayNodes = rayNode;
}

node* requestNode( node** freeRayNodes )
 {
	 node* nodeReq = *freeRayNodes;
	 if ( *freeRayNodes != NULL )
	 {
		 *freeRayNodes = (*freeRayNodes)->next;
	 }
	 //cout << "node prepared. last free " << *freeRayNodes << endl;
	 return nodeReq;
 }

coord square_root( coord entree )
{
	sc_fxtype_params myparamse( BT, BI, sc_dt::SC_TRN, sc_dt::SC_WRAP);
	sc_fxtype_context mycontexte(myparamse);
	sc_dt::sc_fixed< 32, 32, sc_dt::SC_TRN, sc_dt::SC_WRAP > temp1, temp2, temp3, number, n,r;


	number = entree << (BT - BI);

	sc_int<9> count;

		const sc_int<9> maxcount = (BT - BI / 2 - 1);


		r = 0;
	    n = 0;
	    for ( count=0; count <= maxcount; count ++ )
	    {
	       temp1 = n << 2;
	       temp2 = number >> ( BT - 2 );
	       temp3 = 0x03;
	       n = temp1 | ( temp2 & temp3 );
	       number = number << 2;
	       temp1 = r << 2;
	       temp1 = temp1 + sc_int< 2 * BT - BI > (1);
	       if( n - temp1 >= 0 )
	       {
	          n = n - temp1;
	          r = ( r << 1 ) + 1;
	       }
	       else
	       {
	    	 r = r << 1;
	       }
	    }

	    entree = r >> (BT - BI);
	    return entree;
}

int hitTriangle (const ray &r, const triangle &Tr, coord &t){

	vect_fixed e1 = {Tr.V2.x - Tr.V1.x, Tr.V2.y - Tr.V1.y, Tr.V2.z - Tr.V1.z}; //E1
	vect_fixed e2 = {Tr.V3.x - Tr.V1.x, Tr.V3.y - Tr.V1.y, Tr.V3.z - Tr.V1.z}; //E2
	vect_fixed t1 = {r.start.x - Tr.V1.x, r.start.y - Tr.V1.y, r.start.z - Tr.V1.z};//T
	vect_fixed P = {(r.dir.y)*(e2.z)-(r.dir.z)*(e2.y),(e2.x)*(r.dir.z)-(r.dir.x)*(e2.z),(r.dir.x)*(e2.y)-(e2.x)*(r.dir.y)};
	vect_fixed Q = {(t1.y)*(e1.z)-(t1.z)*(e1.y),(e1.x)*(t1.z)-(t1.x)*(e1.z),(t1.x)*(e1.y)-(e1.x)*(t1.y)};
	coord det = P*e1;

	if (det > - 0.001 && det < 0.001)
	{
		return 0;
	}

	coord u = (coord)((t1*P));

	if ( u < 0 || u > 1.0001*det)
	{
		return 0;
	}
	coord v = (coord)((r.dir * Q));

	if ( (v < 0) || ( (u + v) > 1.0001*det) )
	{
		return 0;
	}

	t =(coord)((e2* Q));

	if( t > 0.001*det && t <= 2*det)
	{
		t =(coord)((e2* Q)/det);
		return 1;
	}
	else
	{
		return 0;
	}
}

int FigureIntersection (struct ray r, scene &myScene,  coord &t)
{
	int current=-1;
	coord limit= 2000/LIMIT_SCALE;// 2000
	int lm=myScene.NbTriangles;
	coord u;
	for(int i=0;i < lm ;i++)
	{
 		if (hitTriangle (r,myScene.triangles[i],u)==1)
		{
			if (u < limit)
			{
				current=i;
				limit=u;
			}
		}
	}
	if ( current != -1 ) //if found
	{
		t = limit;
	}
	return current;
}

int IsReflected(ray r, scene &myScene, int current) {

	if (current==-1)
	{
		return 0;
	}

	material cMat = myScene.materials[myScene.triangles[current].materialId];

	if (cMat.reflection==0)
		return 0;
	else
	{
		return 1;
    }
}


int IsRefracted( ray r, scene &myScene, int current)
{

	if ( current == -1 )
		return 0;

	material cMat = myScene.materials[ myScene.triangles[ current ].materialId ];

	if ( cMat.transmission == 0 && cMat.nrefraction <= 1 )
		return 0;
	else
	{
		return 1;
    }
}

vect_fixed NormalTriangle( int current, scene &myScene )
{

	vect_fixed v1 = { myScene.triangles[current].V2.x - myScene.triangles[current].V1.x, myScene.triangles[current].V2.y - myScene.triangles[current].V1.y, myScene.triangles[current].V2.z - myScene.triangles[current].V1.z };
	vect_fixed v2 = { myScene.triangles[current].V2.x - myScene.triangles[current].V3.x, myScene.triangles[current].V3.y - myScene.triangles[current].V1.y, myScene.triangles[current].V3.z - myScene.triangles[current].V1.z };
	vect_fixed normal={(v1.y)*(v2.z)-(v1.z)*(v2.y),(v2.x)*(v1.z)-(v1.x)*(v2.z),(v1.x)*(v2.y)-(v2.x)*(v1.y)};
	coord temp = square_root(normal*normal);
	sc_dt::sc_fixed< 32, 16, sc_dt::SC_TRN, sc_dt::SC_WRAP > temp1  = 1 / temp;
	normal.x=normal.x*temp1;
	normal.y=normal.y*temp1;
	normal.z=normal.z*temp1;
	return normal;
}
ray CreateReflected (ray r, scene &myScene, int current, coord param_t) {

	ray RR={{0,0,0},{0,0,0}};
	coord t=0;

	point_fixed IntersectionPoint= r.start + param_t * r.dir;
	vect_fixed normal = NormalTriangle(current, myScene);
	if (r.dir * normal > 0)
	{
		normal.x=-normal.x;
		normal.y=-normal.y;
		normal.z=-normal.z;
	}

	RR.start=IntersectionPoint;
	//transmission coefficient
	RR.dir=r.dir-2*(normal*r.dir)*normal;
	return RR;
}

ray CreateRefracted (ray r,scene &myScene,int state, int &type, int &state1, int current, coord param_t) {

	ray RR={{0,0,0},{0,0,0}};
	coord t=0;
	int type1=0;
	int state2=0;
	point_fixed IntersectionPoint = r.start + param_t*r.dir;
	vect_fixed normal=NormalTriangle(current, myScene);

	if (r.dir * normal > 0)
	{
		normal.x=-normal.x;
		normal.y=-normal.y;
		normal.z=-normal.z;
	}

	//cout << "OOPS" << endl;
	RR.start=IntersectionPoint;
	//transmission lookup for the direction
	mfp n = myScene.materials[ myScene.triangles[ current ].materialId ].nrefraction;

	if (state == INSIDE)
	{
		n = (mfp)( n / 1.003);
	}
	else
	{
		n = (mfp)( 1.003 / n);
	}

	//total reflection

	coord total_reflection=1-n*n*(1-(r.dir*normal)*(r.dir*normal));

	if(total_reflection<=1)
	{
		RR.dir=n*r.dir-(n*r.dir*normal + square_root(total_reflection) )*normal;
		type1=REFRACTED_RAY;
		if (state==INSIDE)
		{
			state2=OUTSIDE;
		}
		else
		{
			state2=INSIDE;
		}
	}
	else
	{
		RR.dir=r.dir-2*(normal*r.dir)*normal;
		type1=TOTAL_REFLECTED_RAY;
		state2=INSIDE;
	}

	type=type1;
	state1=state2;
	return RR;
}


void initNode( node* curNode, node* parent, ray r, int h, int type, int state)
{
    curNode->parent = parent;
    curNode->v = r;
    curNode->level = h;
    curNode->visited = 0;
    curNode->type = type;
    curNode->left = 0;
    curNode->right = 0;
    curNode->state=state;
}


node *TreeGenerator(node** freeRayNodes, int hmax, node *root, scene &myScene, int current_fig, coord param_t) {
    node * result;
    int type1=REFRACTED_RAY;
    int state1=INSIDE;

    if ( root->level < hmax)
    {
        if ( IsReflected(root->v,myScene, current_fig)==1 && (root->state==OUTSIDE || root->state==ROOT_RAY)) // On est reste ici
        {
            result = requestNode( freeRayNodes );
            if( result == NULL )
            {
            	cout << "no node disponible" << endl;
            	return result;
            }
        	initNode( result, root, CreateReflected(root->v,myScene,current_fig,param_t), root->level + 1, REFLECTED_RAY , OUTSIDE);
            root->left = result;

            return result;
        }

        if (IsRefracted(root->v,myScene,current_fig)==1)
        {
        	result = requestNode( freeRayNodes );
        	if( result == NULL )
        	{
        		cout << " no node disponible " << endl;
        		return result;
        	}
        	ray r_rf=CreateRefracted(root->v,myScene,root->state,type1,state1,current_fig,param_t);
        	initNode( result, root, r_rf, root->level + 1, type1,state1);
            root->right = result;

            return result;
        }

    }

    node * current = root->parent;

    while ( current != 0 )
    {
    	if( IsRefracted( current->v, myScene, current_fig) == 1  )
    		break;
        current = current -> parent;
    }

    if ( current == 0 ) {
        return 0;
    }

    result = requestNode( freeRayNodes );
    if ( result != NULL )
    {
    	ray r_rf=CreateRefracted(current->v,myScene,current->state,type1,state1, current_fig, param_t);
    	initNode( result, current, r_rf, current->level + 1, type1,state1);
       	current->right = result;
    }
    else
    {
    	cout << "no node disponible " << endl;
    }
    return result;
}

color_fixed LightContribution ( node * root, scene &myScene, int current, coord t )
{

	color_fixed c1 = { 0, 0, 0 };

	coord t1=0;


	if (current == -1 )
	{
		return c1;
	}


	point_fixed IntersectionPoint = root->v.start + t*root->v.dir;
	vect_fixed normal = NormalTriangle( current, myScene );
	material IntersectMat = myScene.materials[ myScene.triangles[ current ].materialId ];


	for( unsigned int j = 0; j < myScene.NbLights; ++j )
	{

		coord n1, n2;
		light currentl = myScene.lights[j];
		vect_fixed dist = currentl.pos - IntersectionPoint;// vector length from the intersection point to the l
		n1 = root->v.dir*normal;

		if (n1 > 0) {
			normal.x=-normal.x;
			normal.y=-normal.y;
			normal.z=-normal.z;
		}
		n1 = root->v.dir*normal;

		n2 = dist*normal;
		if( n1 < 0 && n2 > 0)
		{

			ray lightRay;
			lightRay.start = IntersectionPoint;
			lightRay.dir = (1/square_root(dist*dist)) * dist;
			//let's see if there are objects in the middle of the road that can cause reflection or shadow

			// light ray and ray have to be in the same direction so proyection from the light ray in the ray has to have the
			// same direction so cosI positive

			int shadow=0;
			unsigned int lim=myScene.NbTriangles;

			for (unsigned int l=0; l <lim; ++l)
			{
				//first object found.. change after the first object is found
				if ((hitTriangle(lightRay, myScene.triangles[l], t1))==1)
				{
					shadow = 1;
					break;
				}

			}


			//we considered the objects as lambert-surface ones so this means that the Intensity from the incoming ray
			// depends on the angle between the normal

			//if there's no shadow, we considered the current light source effect : Io=In*cos(0);
			if (shadow==0)
			{
				mfp coef=1;
				for(unsigned int q=0; q<= (unsigned int)(root->level); ++q)
				{
					coef*=IntersectMat.reflection;
				}

				if ((root->type==REFLECTED_RAY) || (root->type==ROOT_RAY) || (root->type==TOTAL_REFLECTED_RAY))
				{
					// lambert

					mfp lambert = (lightRay.dir * normal) * coef; // intensity from objects material
					c1.red = c1.red + lambert * currentl.red * IntersectMat.red;
					c1.green = c1.green + lambert * currentl.green * IntersectMat.green;
					c1.blue = c1.blue + lambert * currentl.blue * IntersectMat.blue;

				}

				else if (root->type==REFRACTED_RAY)
				{

					//pseudo beer
					c1.red = c1.red + coef * IntersectMat.red;
					c1.green = c1.green + coef * IntersectMat.green;
					c1.blue = c1.blue + coef * IntersectMat.blue;
				}
		    }
		}

	}

	return c1;
}

 color_fixed raytracer (ray &r, scene &myScene)
{
	// Initialing variables
	color_fixed c1={0,0,0}; //where to keep the color values
	node rayNodes[MAXRAYS];
	node* freeRayNodes;
	freeRayNodes = initFreeNodeList( rayNodes );


	// ray intersection
	coord param_t=0;
	int current_fig=FigureIntersection( r, myScene, param_t );

	//GENERATE TREE
	//cout << "Crear un arbol binario de rayos secundarios para el rayo" << endl;

	node* root = requestNode( &freeRayNodes );

	if ( root != NULL && current_fig!=-1 )
	{
		//cout << "root full" << endl;
		initNode(root, 0, r, 0, 0, 0);
		int depth_max = 5;

		node * courant = root;
		//adding and inserting nodes
		while ( courant != 0 )
		{

			c1 = c1 + LightContribution ( courant, myScene, current_fig, param_t );

			courant = TreeGenerator(&freeRayNodes, depth_max, courant, myScene, current_fig, param_t );

			if (courant !=0)
			{
				param_t=0;
				current_fig=FigureIntersection(courant->v, myScene, param_t );
			}
		}

	}
	else
	{
		cout << "no node available or intersection in triangle" << endl;
	}
	//the tree will be destroyed when exiting the function
	return c1;
}

 bool draw(char* outputName, scene &myScene)
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

    imageFile.put((unsigned char)(myScene.sizex & 0x00FF)).put((unsigned char)((myScene.sizex & 0xFF00) / 256));
    imageFile.put((unsigned char)(myScene.sizey & 0x00FF)).put((unsigned char)((myScene.sizey & 0xFF00) / 256));
    imageFile.put(24);       // 24 bit bitmap
    imageFile.put(0);
    // end of the TGA header

    // Scanning
    for (int y = 0; y < myScene.sizey; y++) {
    for (int x = 0; x < myScene.sizex; x++) {
        //float red = 0, green = 0, blue = 0;
        //float coef = 1.0f;
        //int level = 0;
        color_fixed c1={0,0,0};
        // Cast the ray
        // Because we are not in conic perspective (point of origin)
        // but in orthographic perspective, there is no natural starting point
        // We have to put it far enough to enclose the whole scene
        // but not too far to avoid floating point precision problems (acne and other)
        // 1000.0f seems like a good compromise for now..
    	sc_dt::sc_fixed< 32, 32, sc_dt::SC_TRN, sc_dt::SC_WRAP > x1=x;
    	sc_dt::sc_fixed< 32, 32, sc_dt::SC_TRN, sc_dt::SC_WRAP > y1=y;
    	sc_dt::sc_fixed< 32, 32, sc_dt::SC_TRN, sc_dt::SC_WRAP > z1=-1000;
        ray viewRay = { {(coord)(x1/LIMIT_SCALE),(coord) (y1/LIMIT_SCALE),(coord)(z1/LIMIT_SCALE)}, {0,0,1}};
        c1 = raytracer(viewRay,myScene);
        imageFile.put((unsigned char)min(float(c1.blue)*255.0f,255.0f)).put((unsigned char)min(float(c1.green)*255.0f, 255.0f)).put((unsigned char)min(float(c1.red)*255.0f, 255.0f));
    }
    }
    return true;
 }

//struct that stocks all scene elements
scene myScene;

//all triangle references to be put in
TrigRefList refTrigs[ 2*MAXTRIANGLES ];

//All kdTreeNodes
kdTreeNode nodes[ MAXNODES ];

//For initializing the kdTreeNode Pool

kdTreeNode* initFreeNodeList( kdTreeNode nodes[MAXNODES] )
{
	int i;
	kdTreeNode* freeNodes;
	nodes[0].next = NULL;

	for( i = 1; i < MAXNODES; i++ )
	{
		nodes[i].next = &nodes[i-1];
		//cout << "ni.next " << nodes[i].next << endl;
	}
	freeNodes = &(nodes[MAXNODES - 1]);
	//cout << "last " << freeNodes << endl;
	return freeNodes;
}

//delivers a node to kdTreeNode pool

void deliverNode( kdTreeNode* node, kdTreeNode** freeNodes )
{
	node->next = *freeNodes;
	*freeNodes = node;
}

//request node from the kdTreeNode pool. It is like malloc

kdTreeNode* requestNode( kdTreeNode** freeNodes )
{
	kdTreeNode* nodeReq = *freeNodes;
	if ( freeNodes != NULL )
	{
		*freeNodes = (*freeNodes)->next;
	}
	return nodeReq;
}

//TrigRefList is a pool for stocking the references for the triangles
//inside a kdTree leaf node. The following functions have the same
//kdTreeNode function Purposes

TrigRefList* initFreeNodeList( TrigRefList nodes[MAXTRIGREFS] )
{
	int i;
	TrigRefList* freeNodes;
	nodes[0].next = NULL;

	for( i = 1; i < MAXTRIGREFS; i++ )
	{
		nodes[i].next = &nodes[i-1];
		//cout << "ni.next " << nodes[i].next << endl;
	}
	freeNodes = &(nodes[MAXTRIGREFS - 1]);
	//cout << "last " << freeNodes << endl;
	return freeNodes;
}

void deliverNode( TrigRefList* node, TrigRefList** freeNodes )
{
	node->next = *freeNodes;
	*freeNodes = node;
}

TrigRefList* requestNode( TrigRefList** freeNodes )
{
	TrigRefList* nodeReq = *freeNodes;
	if ( freeNodes != NULL )
	{
		*freeNodes = (*freeNodes)->next;
	}
	return nodeReq;
}

//this function initializes a kdTreeNode after requesting it with requestNode

kdTreeNode* newNode( kdTreeNode** freeNodes, const coord X0, const coord Y0, const coord Z0, const coord X1, const coord Y1, const coord Z1 )
{
	kdTreeNode * newNode;
	//cout << " freeNodes " << *freeNodes << endl;
	newNode = requestNode( freeNodes );
	if( newNode != NULL )
	{
		//cout << "Putting data into the new node" << endl;
		newNode->x0 = X0;
		newNode->y0 = Y0;
		//cout << "Y0" << Y0 << " newNode " << newNode->y0 << endl;
		newNode->x1 = X1;
		newNode->y1 = Y1;
		newNode->z0 = Z0;
		newNode->z1 = Z1;
		newNode->left = NULL;
		newNode->right = NULL;
		newNode->trig = NULL;
		newNode->next = NULL;

		//cout << "x0l "<< newNode->x0 << " x1l " << newNode->x1 << endl;
		//cout << "y0l "<< newNode->y0 << " y1l " << newNode->y1 << endl;
		//cout << "z0l "<< newNode->z0 << " z1l " << newNode->z1 << endl << endl;
	}
	//cout << "returning new node" << endl;
	return newNode;
}
