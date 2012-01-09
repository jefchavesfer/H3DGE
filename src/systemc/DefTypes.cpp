#include "DefTypes.h"


int compLess( coord x, coord y )
{
	coord res = x - y;
	if( x - y < 0.0001 && x - y > 0 )
	{
		return 0; //equal
	}
	else
	{
		if ( x < y )
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
}

//this function initializes a kdTreeNode after requesting it with requestNode
kdTreeNode* newNode( kdTreeNode** freeNodes, const coord X0, const coord Y0, const coord Z0, const coord X1, const coord Y1, const coord Z1 )
{
	kdTreeNode * newNode;
	//printfdeb << " freeNodes " << *freeNodes << endl;
	newNode = requestNode( freeNodes );
	if( newNode != NULL )
	{
		coutdeb << "Putting data into the new node" << endl;
		newNode->x0 = X0;
		newNode->y0 = Y0;
		newNode->x1 = X1;
		newNode->y1 = Y1;
		newNode->z0 = Z0;
		newNode->z1 = Z1;
		newNode->left = NULL;
		newNode->right = NULL;
		newNode->trig = NULL;
		newNode->next = NULL;

		coutdeb << "x0 "<< newNode->x0 << " x1 " << newNode->x1 << endl;
		coutdeb << "y0 "<< newNode->y0 << " y1 " << newNode->y1 << endl;
		coutdeb << "z0 "<< newNode->z0 << " z1 " << newNode->z1 << endl;
	}
	coutdeb << "returning new node" << endl << endl;
	return newNode;
}

//For initializing the kdTreeNode Pool

kdTreeNode* initFreeNodeList( kdTreeNode nodes[MAXNODES] )
{
	int i;
	kdTreeNode* freeNodes;
	nodes[0].next = NULL;

	for( i = 1; i < MAXNODES; i++ )
	{
		nodes[i].next = &nodes[i-1];
		//printfdeb << "ni.next " << nodes[i].next << endl;
	}
	freeNodes = &(nodes[MAXNODES - 1]);
	//printfdeb << "last " << freeNodes << endl;
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



//function for generating a pool of rays

node* initFreeNodeList( node rayNodes[MAXRAYS] )
{
	int i;
	node* freeNodes;
	rayNodes[0].next = NULL;

	for( i = 1; i < MAXRAYS; i++ )
	{
		rayNodes[i].next = &rayNodes[i-1];
		//printfdeb << "ni.next " << nodes[i].next << endl;
	}
	freeNodes = & ( rayNodes[MAXRAYS - 1] );
	//printfdeb << "last " << freeNodes << endl;
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
	//printf("node requested\n");
	return nodeReq;
}


void initNode( node* curNode, node* parent, kdTreeNode* packet, ray r, triangle* trigIntersected, int h, int type, int state)
{
	curNode->parent = parent;
	curNode->v = r;
	curNode->level = h;
	curNode->visited = 0;
	curNode->type = type;
	curNode->left = 0;
	curNode->right = 0;
	curNode->state=state;
	curNode->currentPacket = packet;
	curNode->lastIntercepted = trigIntersected;
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
		//printfdeb << "ni.next " << nodes[i].next << endl;
	}
	freeNodes = &(nodes[MAXTRIGREFS - 1]);
	//printfdeb << "last " << freeNodes << endl;
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



