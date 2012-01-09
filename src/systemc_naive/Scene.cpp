#include "Scene.h"

const vect_fixed NullVector = { 0,0,0 };
const point_fixed Origin = { 0,0,0 };

//helper method - returns squared distance to the node's bounds (0 if inside)
ifixed disq( kdTreeNode* ctr, point_fixed pos)
{
	const ifixed & x = pos.x;
	const ifixed & y = pos.y;
	const ifixed & z = pos.z;
	const ifixed & x0 = ctr->x0;
	const ifixed & y0 = ctr->y0;
	const ifixed & x1 = ctr->x1;
	const ifixed & y1 = ctr->y1;
	const ifixed & z0 = ctr->z0;
	const ifixed & z1 = ctr->z1;

	if(x>x0&&y>y0&&x<x1&&y<y1&&z>z0&&z<z1) return 0.0;
	ifixed dx = 0.0;
	ifixed dy = 0.0;
	ifixed dz = 0.0;
	if(x>x1)
	{
		dx = x-x1;
		if(y<y0)
			dy = y-y0;
		else if(y>y1)
			dy = y-y1;
		if(z<z0)
			dz = z-z0;
		else if(z>z1)
			dz = z-z1;
	}else if(x<x0)
	{
		dx = x-x0;
		if(y<y0)
			dy = y-y0;
		else if(y>y1)
			dy = y-y1;
		if(z<z0)
			dz = z-z0;
		else if(z>z1)
			dz = z-z1;
	}else if(z<z0)
	{
		dz = z-z0;
		if(y<y0)
			dy = y-y0;
		else if(y>y1)
			dy = y-y1;
	}else if(z>z1)
	{
		dz = z-z1;
		if(y<y0)
			dy = y-y0;
		else if(y>y1)
			dy = y-y1;
	}else
	{
		if(y<y0)
			dy = y-y0;
		else if(y>y1)
			dy = y-y1;
	}
	return dx*dx+dy*dy+dz*dz;
}


//for inserting a new candidate inside the spliting point list
void InsertSplitPos( SplitList** sList, SplitList** splitPool, coord splitPos )
{
	// insert a split position candidate in the list if unique
	SplitList* entry = *splitPool;
	SplitList* p = *sList;
	*splitPool = (*splitPool)->next;
	entry->next = 0;
	entry->splitpos = splitPos;
	entry->n1count = 0;
	entry->n2count = 0;
	if (!p )
	{
		*sList = entry;
		//cout << "splitpos " << splitPos << " inserted first " << endl;
	}
	else
	{
		if ( splitPos < p->splitpos )
		{
			entry->next = p;
			*sList = entry;
			//cout << "splitpos " << splitPos << " comp " << p->splitpos << " inserted lowest case" << endl;
		}
		else if ( splitPos == p->splitpos )
		{
			entry->next = *splitPool; // redundant; recycle
			*splitPool = entry;
			//cout << "splitpos " << splitPos << " comp " << p->splitpos <<  " redundant equal case" << endl;
		}
		else
		{
			while ( ( p->next ) )
			{
				if ( splitPos < p->next->splitpos ) break;
				if (splitPos == p->next->splitpos)
				{
					entry->next = *splitPool; // redundant; recycle
					*splitPool = entry;
					//cout << "splitpos " << splitPos << " comp " << p->next->splitpos << " redundant equal iterative case " << endl;
					return;
				}
				p = p->next;
			}
			entry->next = p->next;
			p->next = entry;
			//cout << "splitpos " << splitPos << " comp " << p->splitpos << " inserted iterative case " << endl;
		}
	}
}


//kd-tree generation

bool insersectBox( ifixed limits[6], triangle* t )
{
	if( t->big.x >= limits[0] && t->small.x <= limits[3] &&
		t->big.y >= limits[1] && t->small.y <= limits[4] &&
		t->big.z >= limits[2] && t->small.z <= limits[5] )
	{
		//printf( "intersect!\n" );
		return true;
	}
	else
	{
		//printf( "not intersect!\n" );
		return false;
	}
}

//These vectors are outside the function built because of a stack error

//for guarding the adresses for each brach
triangle *tleft[ MAXTRIGREFS ], *tright[ MAXTRIGREFS ];

//stocks all possible splitting points
SplitList splits[ MAXTRIGREFS ];

bool build( TrigRefList** trianglelist, TrigRefList** freeRefNodes, kdTreeNode** freeNodes, kdTreeNode * tree, const int depth )
{
	kdTreeNode* ctrAnt;            //current node level list pointer
	kdTreeNode* ctrAntBegin;       //current node level list begin
	kdTreeNode* ctr;               //new node level list pointer
	kdTreeNode* ctrBegin;          //new node level list begin
	coord l1, l2, l3, l4;         //limits of chosen splitting position
	int cdepth = 0;                //current depth of the tree
	unsigned int bestn1count, bestn2count;  //best division elements
	//pointers for splitlist management
	SplitList *sList = NULL;
	SplitList *sPool = NULL;

	ctrAnt = tree;
	ctrAntBegin = tree;
	ctr = NULL;
	ctrBegin = NULL;

	//printf( "cdepth  %d depth %d \n", cdepth, depth );
 	do
	{
		do
		{
			if( cdepth == depth || ctrAnt->pnum <= 2 )
			{
				//printf( "Max Depth or few nodes stopping condition\n");
		//		printf( "put %d triangles in the tree\n", ctrAnt->pnum );

				//cout << "index of first triangle: " << ctrAnt->trig->ref->index << endl;
				//cout << "number of triangles: " << ctrAnt->pnum << endl;

				//cout << "null sons " << ctrAnt->left << "    " << ctrAnt->right << endl;
				//printf( "leaf iteration done\n\n" );
			}
			else
			{
				//printf( "building new nodes. Number of triangles: %d\n", ctrAnt->pnum );

				int axis;
				//splits along the axis of who the tree's bound is largest
				if ( ( ctrAnt->x1 - ctrAnt->x0 > ctrAnt->y1 - ctrAnt->y0 ) && ( ctrAnt->x1 - ctrAnt->x0 > ctrAnt->z1 - ctrAnt->z0 ) )
				{
					axis = 0;
					l1 = ctrAnt->x0; l2 = ctrAnt->x1;
				}
				else if ( ( ctrAnt->y1 - ctrAnt->y0 > ctrAnt->z1 - ctrAnt->z0 ) && ( ctrAnt->y1 - ctrAnt->y0 > ctrAnt->z1 - ctrAnt->z0 ) )
				{
					axis = 1;
					l1 = ctrAnt->y0; l2 = ctrAnt->y1;
				}
				else
				{
					axis = 2;
					l1 = ctrAnt->z0; l2 = ctrAnt->z1;
				}

				ctrAnt->spx = axis;
				//printf( "split axis: %d\n", ctrAnt->spx );

				// make a list of the split position candidates
				TrigRefList* l = ctrAnt->trig;

				//initializing bounded splitposlist
				sPool = splits;
				sPool->n1count = 0;
				sPool->n2count = 0;
				sPool->next = NULL;

				for( int i = 1; i < 2 * tree->pnum; i++ )
				{
					sPool = &splits[i];
					sPool->n1count = 0;
					sPool->n2count = 0;
					sPool->next = &splits[i - 1];
				}

				sList = NULL;

				int aidx = 0;
				//cout << "l1 " << l1 << " l2 " << l2 << endl;
				while ( aidx < ctrAnt->pnum )
				{
					if( axis == 0 )
					{
						if( l->ref->V1.x > l1 && l->ref->V1.x < l2 ) InsertSplitPos( &sList, &sPool, l->ref->V1.x );
						if( l->ref->V2.x > l1 && l->ref->V2.x < l2 ) InsertSplitPos( &sList, &sPool, l->ref->V2.x );
						if( l->ref->V3.x > l1 && l->ref->V3.x < l2 ) InsertSplitPos( &sList, &sPool, l->ref->V3.x );
					}
					else if ( axis == 1 )
					{
						if( l->ref->V1.y > l1 && l->ref->V1.y < l2 ) InsertSplitPos( &sList, &sPool, l->ref->V1.y );
						if( l->ref->V2.y > l1 && l->ref->V2.y < l2 ) InsertSplitPos( &sList, &sPool, l->ref->V2.y );
						if( l->ref->V3.y > l1 && l->ref->V3.y < l2 ) InsertSplitPos( &sList, &sPool, l->ref->V3.y );
					}
					else
					{
						if( l->ref->V1.z > l1 && l->ref->V1.z < l2 ) InsertSplitPos( &sList, &sPool, l->ref->V1.z );
						if( l->ref->V2.z > l1 && l->ref->V2.z < l2 ) InsertSplitPos( &sList, &sPool, l->ref->V2.z );
						if( l->ref->V3.z > l1 && l->ref->V3.z < l2 ) InsertSplitPos( &sList, &sPool, l->ref->V3.z );
					}

					l = l->next;
					aidx++;
				}

				// determine n1count / n2count for each split position
				coord b1[6], b2[6], b3[6],b4[6];
				b1[0] = b2[0] = b3[0] = b4[0] = ctrAnt->x0;
				b1[1] = b2[1] = b3[1] = b4[1] = ctrAnt->y0;
				b1[2] = b2[2] = b3[2] = b4[2] = ctrAnt->z0;
				b1[3] = b2[3] = b3[3] = b4[3] = ctrAnt->x1;
				b1[4] = b2[4] = b3[4] = b4[4] = ctrAnt->y1;
				b1[5] = b2[5] = b3[5] = b4[5] = ctrAnt->z1;

				SplitList* splist = sList;

				l1 = ( axis == 0 ) ? b3[0] : ( (axis == 1 ) ? b3[1] : b3[2] );
				l4 = ( axis == 0 ) ? b4[3] : ( (axis == 1 ) ? b4[4] : b4[5] );

				//cout << "l1 " << l1 << " l4 " << l4 << endl;
				while ( splist )
				{
					b4[ axis ] = splist->splitpos;
					b3[ axis + 3 ]= splist->splitpos;
					l2 = b3[ axis + 3 ];
					l3 = b4[ axis ];
					//cout << "l2 " << l2 << " l3 " << l3 << endl;
					TrigRefList* t = ctrAnt->trig;
					//cout << endl;

					for ( int i = 0; i < ctrAnt->pnum; i++ )
					{
						coord ltrigmin, ltrigmax;

						ltrigmin = ( axis == 0 ) ? t->ref->small.x : ( axis == 1 ) ? t->ref->small.y : t->ref->small.z;
						ltrigmax = ( axis == 0 ) ? t->ref->big.x : ( axis == 1 ) ? t->ref->big.y : t->ref->big.z;
						//printf ( "\n" );
						//printf ( "trig %d\n", t->ref->index );
						//cout << "l1 " << l1 << " l2 " << l2 << endl;
						//cout << "l3 " << l3 << " l4 " << l4 << endl;
						//cout << "ltrigmin: " << ltrigmin << " ltrigmax: " << ltrigmax << endl;
						if ( ( ltrigmin <= l2 ) && ( ltrigmax >= l1 ) )
						{
							if( insersectBox( b3, t->ref ) )
							{
								splist->n1count++;
								//printf( "n1count++\n" );
							}
						}
						if ( ( ltrigmin <= l4 ) && ( ltrigmax >= l3) )
						{
							if( insersectBox( b4, t->ref ) )
							{
								splist->n2count++;
								//printf( "n2count++\n" );
							}
						}
						t = t->next;
					}
					splist = splist->next;
				}

				// calculate surface area for current node
				surfAreaTot SAV = 0.5 / ( ( ctrAnt->x1 - ctrAnt->x0 ) * ( ctrAnt->y1 - ctrAnt->y0 ) + ( ctrAnt->y1 - ctrAnt->y0 ) * ( ctrAnt->z1 - ctrAnt->z0 ) + ( ctrAnt->x1 - ctrAnt->x0 ) * ( ctrAnt->z1 - ctrAnt->z0 ) );
				//cout << " SAV " << SAV << endl;

				// calculate cost for not splitting
				cost cleaf = ctrAnt->pnum * 1.0;

				// determine optimal split plane position
				cost lowcost = MAXTRIANGLES;
				coord bestpos = 0;
				splist = sList;
				while (splist)
				{
					// calculate child node extends
					b4[axis] = splist->splitpos;
					b3[axis + 3] = splist->splitpos;

					//cout << "splitpos " << splist->splitpos << endl;
					//cout << "n1count " << splist->n1count << " n2count " << splist->n2count << endl;
					//cout << "b3[0] " << b3[0] << " b3[1] " << b3[1] << " b3[2] " << b3[2] << " b3[3] " << b3[3] << " b3[4] " << b3[4] <<  " b3[5] " << b3[5] << endl;
					//cout << "b4[0] " << b4[0] << " b4[1] " << b4[1] << " b4[2] " << b4[2] << " b4[3] " << b4[3] << " b4[4] " << b4[4] <<  " b4[5] " << b4[5] << endl;

					// calculate child node cost
					surfArea SA1 = 2 * (( b3[3] - b3[0] ) * ( b3[4] - b3[1] ) + ( b3[3] - b3[0] ) * ( b3[5] - b3[2] ) + ( b3[4] - b3[1] ) * ( b3[5] - b3[2] ) );
					surfArea SA2 = 2 * (( b4[3] - b4[0] ) * ( b4[4] - b4[1] ) + ( b3[3] - b3[0] ) * ( b4[5] - b4[2] ) + ( b4[4] - b4[1] ) * ( b4[5] - b4[2] ) );
					//cout << "SA1 " << SA1 << " SA2 " << SA2 << endl;

					cost splitcost = 0.3 + 1.0 * (SA1 * SAV * splist->n1count + SA2 * SAV * splist->n2count);
					//cout << lowcost;
					//cout << "split cost! " << splitcost << "low cost" << lowcost << endl;
					//cout << "aaa" << endl;
					// update best cost tracking variables
					if (splitcost < lowcost)
					{
						//cout<< "!";
						lowcost = splitcost;
						//cout<< "0 ";
						bestpos = splist->splitpos;
						//cout << "1";
						b1[axis + 3] = bestpos;
						//cout << "2";
						b2[axis] = bestpos;
						//cout<<" 3 ";
						bestn1count = splist->n1count;
						bestn2count = splist->n2count; //best division elements
					}
					//cout<<"eeee";
					splist = splist->next;
				}
				//cout << "1";
				if (lowcost < cleaf)
				{
					// construct child nodes
					unsigned int n1count = 0, n2count = 0, total = 0;
					// assign primitives to both sides
					l1 = ( axis == 0) ? b1[0] : (axis == 1 ) ? b1[1] : b1[2];
					l2 = ( axis == 0) ? b1[3] : (axis == 1 ) ? b1[4] : b1[5];
					l3 = ( axis == 0) ? b2[0] : (axis == 1 ) ? b2[1] : b2[2];
					l4 = ( axis == 0) ? b2[3] : (axis == 1 ) ? b2[4] : b2[5];
					l = ctrAnt->trig;
					//cout << "2";
					for ( int i = 0; i < ctrAnt->pnum; i++ )
					{
						coord ltrigmin, ltrigmax;
						ltrigmin = ( axis == 0 ) ? l->ref->small.x : ( axis == 1 ) ? l->ref->small.y : l->ref->small.z;
						ltrigmax = ( axis == 0 ) ? l->ref->big.x   : ( axis == 1 ) ? l->ref->big.y   : l->ref->big.z;

						//printf( "trig index %d\n", l->ref->index );
						//cout << "ltrigmin " << ltrigmin << " ltrigmax " << ltrigmax <<endl;
						//cout << "l1 " << l1 << " l2 " << l2 << " l3 " << l3 << " l4 " << l4 << endl;

						if ( (ltrigmin <= l2) && ( ltrigmax >= l1))
						{
							if( insersectBox( b1, l->ref ) )
							{
								tleft[n1count] = l->ref;
								n1count++;
							}
						}
						if ((ltrigmin <= l4) && (ltrigmax >= l3))
						{
							if( insersectBox( b2, l->ref ) )
							{
								tright[n2count] = l->ref;
								n2count++;
							}
						}
						TrigRefList* temp = l;

						l = l->next;
						total++;
						deliverNode( temp, freeRefNodes );
					}
					//the father is no more a leave
					ctrAnt->trig = NULL;

					//now putting the original

					//printf( "split: %d\n", ctrAnt->spx );

					ctrAnt->left = newNode( freeNodes, b1[0], b1[1], b1[2], b1[3], b1[4], b1[5] );
					ctrAnt->right = newNode( freeNodes, b2[0], b2[1], b2[2], b2[3], b2[4], b2[5] );

					if( ctrAnt->left == NULL || ctrAnt->right == NULL )
					{
						//printf("No free Node\n");
						return false;
					}
					ctrAnt->left->pnum = n1count;
					ctrAnt->right->pnum = n2count;

					if( n1count > 0 )
					{
						l = requestNode( freeRefNodes );
						if( l )
						{
							l->next = 0;
							l->ref = tleft[0];
							ctrAnt->left->trig = l;
						}
						else
						{
							//printf( "no disponible refs for left node\n" );
							break;
						}
					}
					for( unsigned int i = 1; i < n1count; i++ )
					{
						l = requestNode( freeRefNodes );
						if( l )
						{
							l->next = ctrAnt->left->trig;
							l->ref = tleft[i];
							ctrAnt->left->trig = l;
						}
						else
						{
							//printf( "no disponible refs for left node\n" );
							break;
						}
					}

					if( n2count > 0 )
					{
						l = requestNode( freeRefNodes );
						if ( l )
						{
							l->next = 0;
							l->ref = tright[0];
							ctrAnt->right->trig = l;
						}
						else
						{
							//printf( "no disponible refs for right node\n" );
							break;
						}
					}
					for( unsigned int i = 1; i < n2count; i++ )
					{
						l = requestNode( freeRefNodes );
						if ( l )
						{
							l->next = ctrAnt->right->trig;
							l->ref = tright[i];
							ctrAnt->right->trig = l;
						}
						else
						{
							//printf( "no disponible refs for right node\n" );
							break;
						}
					}
					//printf( "next nodes elements; left: %d right: %d\n", ctrAnt->left->pnum, ctrAnt->right->pnum );

					ctrAnt->left->next = ctrAnt->right;

					if( ctrBegin == NULL )
					{
						///printf( "first new level node\n" );
						ctrBegin = ctrAnt->left;
					}
					else
					{
						//printf( "bounding previous new level node\n" );
						ctr->next = ctrAnt->left;
					}
					//printf( "pendent bound\n " );
					ctr = ctrAnt->right;
					///printf( "new sub nodes:\n" );
					//printf( "numl : %d numr: %d cdepth: %d\n", ctrAnt->left->pnum, ctrAnt->right->pnum, cdepth );
					//printf( "build iteration done!\n\n" );
				}
				else
				{
					//printf( "NOT WORTHY SPLITTING\n" );
				}
			}
			ctrAnt = ctrAnt->next;
			if( ctrAnt != NULL )
			{
				//printf( "next one: %d\n", ctrAnt->pnum );
			}
		} while (ctrAnt != NULL);
		if(ctr)
		{
			ctr->next = NULL;
			//printf( "pendent bound = NULL\n");
		}

		//printf( "going to next tree level\n" );
		//printf( "printing next level elements:\n" );

		ctrAnt = ctrBegin;
		while ( ctrAnt != NULL )
		{
			//printf( "pnum: %d\n", ctrAnt->pnum );
			ctrAnt = ctrAnt->next;
		}

		ctrAnt = ctrBegin;
		ctrBegin = NULL;
		ctr = NULL;
		cdepth++;
	} while ( ctrAnt != NULL );
	//printf( "tree built\n\n" );
 	return true;
}

//kd-tree traversal
kdTreeNode * traverse(kdTreeNode * const ctr, const point_fixed  pos)
{
	if( ctr == NULL ) return NULL;
	if( ctr->trig != NULL )
	{
		//cout << "arrived to node" << endl;
		return ctr;
	}
	if( ctr->spx == 0 )
	{
		//cout << "traversing x axis: " << ctr->left->x1 << endl;
		if( pos.x < ctr->left->x1 )
		{
			//cout << "left" << endl;
			return traverse( ctr->left, pos );
		}
		else
		{
			//cout << "right" << endl;
			return traverse( ctr->right, pos );
		}
	}
	else if( ctr->spx == 1 )
	{
		//cout << "traversing y axis: " << ctr->left->y1 << endl;
		if( pos.y < ctr->left->y1 )
		{
			//cout << "left" << endl;
			return traverse( ctr->left, pos );
		}
		else
		{
			//cout << "right" << endl;
			return traverse( ctr->right, pos );
		}
	}
	else
	{
		//cout << "traversing z axis: " << ctr->left->z1 << endl;
		if(pos.z < ctr->left->z1)
		{
			//cout << "left" << endl;
			return traverse(ctr->left,pos);
		}
		else
		{
			//cout << "right" << endl;
			return traverse(ctr->right,pos);
		}
	}
}

int getKdNodeIntersections( ray_fixed* ray, kdTreeNode* i, pairInt intersections[6] )
{
	//normal vectors
	vect_fixed nx = { 1, 0, 0 };
	vect_fixed ny = { 0, 1, 0 };
	vect_fixed nz = { 0, 0, 1 };

	//cout << "i->x0: " << i->x0 << " i->x1 " << i->x1 << " i->y0 " << i->y0 << " i->y1 " << i->y1 << " i->z0 " << i->z0  << " i->z1 " << i->z1 << endl;
	//plan points
	point_fixed vx0 = { i->x0, 0 ,0 };
	point_fixed vx1 = { i->x1, 0 ,0 };
	point_fixed vy0 = { 0, i->y0, 0 };
	point_fixed vy1 = { 0, i->y1, 0 };
	point_fixed vz0 = { 0, 0, i->z0 };
	point_fixed vz1 = { 0, 0, i->z1 };

	//cout << "ray start x " << ray->start.x << " y " << ray->start.y << " z " << ray->start.z << endl;
	//cout << "ray dir x " << ray->dir.x << " y " << ray->dir.y << " z " << ray->dir.z << endl;

	vect_fixed wx0, wx1, wy0, wy1, wz0, wz1;
	ifixed Nx0, Nx1, Ny0, Ny1, Nz0, Nz1;
	ifixed sx0, sx1, sy0, sy1, sz0, sz1;

	intersections[0].valid = false;
	intersections[1].valid = false;
	intersections[2].valid = false;
	intersections[3].valid = false;
	intersections[4].valid = false;
	intersections[5].valid = false;

	ifixed Dx = nx * ray->dir;
	//plan parallel ray
	if( Dx < EPSILON ) {
		intersections[0].I.x = 0; intersections[0].I.y = 0; intersections[0].I.z = 0;
		intersections[1].I.x = 0; intersections[1].I.y = 0; intersections[1].I.z = 0;
	}
	else
	{
		wx0 = ray->start - vx0;
		Nx0 = - ( nx * wx0 );
		sx0 = Nx0 / Dx;
		wx1 = ray->start - vx1;
		Nx1 = - ( nx * wx1 );
		sx1 = Nx1 / Dx;
		intersections[0].I = ray->start + ( sx0 * ray->dir );
		intersections[1].I = ray->start + ( sx1 * ray->dir );
		if(intersections[0].I.x >= i->x0 && intersections[0].I.y >= i->y0 && intersections[0].I.z >= i->z0 && intersections[0].I.x <= i->x1 && intersections[0].I.y <= i->y1 && intersections[0].I.z <= i->z1)
		{
			intersections[0].valid = true;
		}
		if(intersections[1].I.x >= i->x0 && intersections[1].I.y >= i->y0 && intersections[1].I.z >= i->z0 && intersections[1].I.x <= i->x1 && intersections[1].I.y <= i->y1 && intersections[1].I.z <= i->z1)
		{
			intersections[1].valid = true;
		}
	}
	ifixed Dy = ny * ray->dir;
	if( Dy < EPSILON )
	{
		intersections[2].I.x = 0; intersections[2].I.y = 0; intersections[2].I.z = 0;
		intersections[3].I.x = 0; intersections[3].I.y = 0; intersections[3].I.z = 0;

	}
	else
	{
		wy0 = ray->start - vy0;
		Ny0 = - ( ny * wy0 );
		sy0 = Ny0 / Dy;
		wy1 = ray->start - vy1;
		Ny1 = - ( ny * wy1 );
		sy1 = Ny1 / Dy;
		intersections[2].I = ray->start + ( sy0 * ray->dir );
		intersections[3].I = ray->start + ( sy1 * ray->dir );
		if(intersections[2].I.x >= i->x0 && intersections[2].I.y >= i->y0 && intersections[2].I.z >= i->z0 && intersections[2].I.x <= i->x1 && intersections[2].I.y <= i->y1 && intersections[2].I.z <= i->z1)
		{
			intersections[2].valid = true;
		}
		if(intersections[3].I.x >= i->x0 && intersections[3].I.y >= i->y0 && intersections[3].I.z >= i->z0 && intersections[3].I.x <= i->x1 && intersections[3].I.y <= i->y1 && intersections[3].I.z <= i->z1)
		{
			intersections[3].valid = true;
		}
	}
	ifixed Dz = nz * ray->dir;
	if( Dz < EPSILON ) {
		intersections[4].I.x = 0; intersections[4].I.y = 0; intersections[4].I.z = 0;
		intersections[5].I.x = 0; intersections[5].I.y = 0; intersections[5].I.z = 0;
	}
	else
	{
		wz0 = ray->start - vz0;
		Nz0 = - ( nz * wz0 );
		sz0 = Nz0 / Dz;
		wz1 = ray->start - vz1;
		Nz1 = - ( nz * wz1 );
		sz1 = Nz1 / Dz;
		intersections[4].I = ray->start + ( sz0 * ray->dir );
		intersections[5].I = ray->start + ( sz1 * ray->dir );
		if(intersections[4].I.x >=i->x0 && intersections[4].I.y >= i->y0 && intersections[4].I.z >= i->z0 && intersections[0].I.x <= i->x1 && intersections[4].I.y < i->y1 && intersections[4].I.z <= i->z1)
		{
			intersections[4].valid = true;
		}
		if(intersections[5].I.x >=i->x0 && intersections[5].I.y >= i->y0 && intersections[5].I.z >= i->z0 && intersections[1].I.x <= i->x1 && intersections[5].I.y <= i->y1 && intersections[5].I.z <= i->z1)
		{
			intersections[5].valid = true;
		}
	}

	//verifying if there is equal intersection points (edges of the box
	for( int k = 0; k < 5; k ++ )
	{
		if( intersections[k].valid == false )
			continue;
		for( int l = k + 1; l < 6; l++ )
		{
			if( intersections[l].valid == false )
				continue;
			if( intersections[k].I.x == intersections[l].I.x &&
				intersections[k].I.y == intersections[l].I.y &&
				intersections[k].I.z == intersections[l].I.z
			   )
			{
				intersections[l].valid = false;
			}
		}
	}

	//cout << "Dx " << Dx << " Dy " << Dy << " Dz " << Dz << endl;

	//if zero ray inside plan
	//to do: treat this case

	//cout << "Nx0 " << Nx0 << " Nx1 " << Nx1 << " Ny0 " << Ny0 << " Ny1 " << Ny1 << " Nz0 " << Nz0 << " Nz1 " << Nz1 << endl;
	//cout << "sx0 " << sx0 << " sx1 " << sx1 << " sy0 " << sy0 << " sy1 " << sy1 << " sz0 " << sz0 << " sz1 " << sz1 << endl;

	//cout << "int x0 x " << intersections[0].I.x << " y " <<  intersections[0].I.y << " z " << intersections[0].I.z << endl;
	//cout << "intx0valid " << intersections[0].valid << endl;

	//cout << "int x1 x " << intersections[1].I.x << " y " <<  intersections[1].I.y << " z " << intersections[1].I.z << endl;
	//cout << "intx1valid " << intersections[1].valid << endl;

	//cout << "int y0 x " << intersections[2].I.x << " y " <<  intersections[2].I.y << " z " << intersections[2].I.z << endl;
	//cout << "inty0valid " << intersections[2].valid << endl;

	//cout << "int y1 x " << intersections[3].I.x << " y " <<  intersections[3].I.y << " z " << intersections[3].I.z << endl;
	//cout << "inty1valid " << intersections[3].valid << endl;

	//cout << "int z0 x " << intersections[4].I.x << " y " <<  intersections[4].I.y << " z " << intersections[4].I.z << endl;
	//cout << "intz0valid " << intersections[4].valid << endl;

	//cout << "int z1 x " << intersections[5].I.x << " y " <<  intersections[5].I.y << " z " << intersections[5].I.z << endl;
	//cout << "intz1valid " << intersections[5].valid << endl;
	return 0;
}


void getNodeInterPoints( ray_fixed* ray, kdTreeNode* i, kdTreeNode* tree, pairInt intersections[6] )
{
	//normal vectors
	vect_fixed nx = { 1, 0, 0 };
	vect_fixed ny = { 0, 1, 0 };
	vect_fixed nz = { 0, 0, 1 };

	//cout << "i->x0: " << i->x0 << " i->x1 " << i->x1 << " i->y0 " << i->y0 << " i->y1 " << i->y1 << " i->z0 " << i->z0  << " i->z1 " << i->z1 << endl;
	//plan points
	point_fixed vx0 = { i->x0, 0 ,0 };
	point_fixed vx1 = { i->x1, 0 ,0 };
	point_fixed vy0 = { 0, i->y0, 0 };
	point_fixed vy1 = { 0, i->y1, 0 };
	point_fixed vz0 = { 0, 0, i->z0 };
	point_fixed vz1 = { 0, 0, i->z1 };

	//cout << "ray start x " << ray->start.x << " y " << ray->start.y << " z " << ray->start.z << endl;
	//cout << "ray dir x " << ray->dir.x << " y " << ray->dir.y << " z " << ray->dir.z << endl;

	vect_fixed wx0, wx1, wy0, wy1, wz0, wz1;
	ifixed Nx0, Nx1, Ny0, Ny1, Nz0, Nz1;
	ifixed sx0, sx1, sy0, sy1, sz0, sz1;

	intersections[0].valid = false;
	intersections[1].valid = false;
	intersections[2].valid = false;
	intersections[3].valid = false;
	intersections[4].valid = false;
	intersections[5].valid = false;

	ifixed Dx = nx * ray->dir;
	//plan parallel ray
	if( Dx < EPSILON ) {
		intersections[0].I.x = 0; intersections[0].I.y = 0; intersections[0].I.z = 0;
		intersections[1].I.x = 0; intersections[1].I.y = 0; intersections[1].I.z = 0;
	}
	else
	{
		wx0 = ray->start - vx0;
		Nx0 = - ( nx * wx0 );
		sx0 = Nx0 / Dx;
		if( sx0 > EPSILON )
		{
			intersections[0].I = ray->start + ( sx0 * ray->dir );
			if(intersections[0].I.x >= i->x0 && intersections[0].I.y >= i->y0 && intersections[0].I.z >= i->z0 && intersections[0].I.x <= i->x1 && intersections[0].I.y <= i->y1 && intersections[0].I.z <= i->z1)
			{
				intersections[0].valid = true;
			}
		}
		else
		{
			intersections[0].I.x = 0; intersections[0].I.y = 0; intersections[0].I.z = 0;
		}
		wx1 = ray->start - vx1;
		Nx1 = - ( nx * wx1 );
		sx1 = Nx1 / Dx;
		if( sx1 > EPSILON )
		{
			intersections[1].I = ray->start + ( sx1 * ray->dir );

			if(intersections[1].I.x >= i->x0 && intersections[1].I.y >= i->y0 && intersections[1].I.z >= i->z0 && intersections[1].I.x <= i->x1 && intersections[1].I.y <= i->y1 && intersections[1].I.z <= i->z1)
			{
				intersections[1].valid = true;
			}
		}
		else //if s is <= 0. It means that is the point already found or the ray is not going off the triangle pack
		{

			intersections[1].I.x = 0; intersections[1].I.y = 0; intersections[1].I.z = 0;
		}
	}
	ifixed Dy = ny * ray->dir;
	if( Dy < EPSILON )
	{
		intersections[2].I.x = 0; intersections[2].I.y = 0; intersections[2].I.z = 0;
		intersections[3].I.x = 0; intersections[3].I.y = 0; intersections[3].I.z = 0;
	}
	else
	{
		wy0 = ray->start - vy0;
		Ny0 = - ( ny * wy0 );
		sy0 = Ny0 / Dy;
		if( sx1 > EPSILON )
		{
			intersections[2].I = ray->start + ( sy0 * ray->dir );

			if(intersections[2].I.x >= i->x0 && intersections[2].I.y >= i->y0 && intersections[2].I.z >= i->z0 && intersections[2].I.x <= i->x1 && intersections[2].I.y <= i->y1 && intersections[2].I.z <= i->z1)
			{
				intersections[2].valid = true;
			}
		}
		else //if s is <= 0. It means that is the point already found or the ray is not going off the triangle pack
		{

			intersections[2].I.x = 0; intersections[2].I.y = 0; intersections[2].I.z = 0;
		}
		wy1 = ray->start - vy1;
		Ny1 = - ( ny * wy1 );
		sy1 = Ny1 / Dy;
		if( sy1 > EPSILON )
		{
			intersections[3].I = ray->start + ( sy1 * ray->dir );
			if(intersections[3].I.x >= i->x0 && intersections[3].I.y >= i->y0 && intersections[3].I.z >= i->z0 && intersections[3].I.x <= i->x1 && intersections[3].I.y <= i->y1 && intersections[3].I.z <= i->z1)
			{
				intersections[3].valid = true;
			}
		}
		else //if s is <= 0. It means that is the point already found or the ray is not going off the triangle pack
		{
			intersections[3].I.x = 0; intersections[3].I.y = 0; intersections[3].I.z = 0;
		}
	}
	ifixed Dz = nz * ray->dir;
	if( Dz < EPSILON )
	{
		intersections[4].I.x = 0; intersections[4].I.y = 0; intersections[4].I.z = 0;
		intersections[5].I.x = 0; intersections[5].I.y = 0; intersections[5].I.z = 0;
	}
	else
	{
		wz0 = ray->start - vz0;
		Nz0 = - ( nz * wz0 );
		sz0 = Nz0 / Dz;
		if( sz0 > EPSILON )
		{
			intersections[4].I = ray->start + ( sz0 * ray->dir );
			if(intersections[4].I.x >=i->x0 && intersections[4].I.y >= i->y0 && intersections[4].I.z >= i->z0 && intersections[4].I.x <= i->x1 && intersections[4].I.y < i->y1 && intersections[4].I.z <= i->z1)
			{
				intersections[4].valid = true;
			}
		}
		else
		{
			intersections[4].I.x = 0; intersections[4].I.y = 0; intersections[4].I.z = 0;
		}
		wz1 = ray->start - vz1;
		Nz1 = - ( nz * wz1 );
		sz1 = Nz1 / Dz;
		if( sz1 > EPSILON )
		{
			intersections[5].I = ray->start + ( sz1 * ray->dir );
			if(intersections[5].I.x >=i->x0 && intersections[5].I.y >= i->y0 && intersections[5].I.z >= i->z0 && intersections[5].I.x <= i->x1 && intersections[5].I.y <= i->y1 && intersections[5].I.z <= i->z1)
			{
				intersections[5].valid = true;
			}
		}
		else
		{
			intersections[5].I.x = 0; intersections[5].I.y = 0; intersections[5].I.z = 0;
		}
	}
	//cout << "Dx " << Dx << " Dy " << Dy << " Dz " << Dz << endl;

	//verifying if there is equal intersection points (edges of the box
	for( int k = 0; k < 5; k ++ )
	{
		if( intersections[k].valid == false )
			continue;
		for( int l = k + 1; l < 6; l++ )
		{
			if( intersections[l].valid == false )
				continue;
			if( intersections[k].I.x == intersections[l].I.x &&
					intersections[k].I.y == intersections[l].I.y &&
					intersections[k].I.z == intersections[l].I.z
			)
			{
				intersections[l].valid = false;
			}
		}
	}

	//cout << "int x0 x " << intersections[0].I.x << " y " <<  intersections[0].I.y << " z " << intersections[0].I.z << endl;
	//cout << "intx0valid " << intersections[0].valid << endl;

	//cout << "int x1 x " << intersections[1].I.x << " y " <<  intersections[1].I.y << " z " << intersections[1].I.z << endl;
	//cout << "intx1valid " << intersections[1].valid << endl;

	//cout << "int y0 x " << intersections[2].I.x << " y " <<  intersections[2].I.y << " z " << intersections[2].I.z << endl;
	//cout << "inty0valid " << intersections[2].valid << endl;

	//cout << "int y1 x " << intersections[3].I.x << " y " <<  intersections[3].I.y << " z " << intersections[3].I.z << endl;
	//cout << "inty1valid " << intersections[3].valid << endl;

	//cout << "int z0 x " << intersections[4].I.x << " y " <<  intersections[4].I.y << " z " << intersections[4].I.z << endl;
	//cout << "intz0valid " << intersections[4].valid << endl;

	//cout << "int z1 x " << intersections[5].I.x << " y " <<  intersections[5].I.y << " z " << intersections[5].I.z << endl;
	//cout << "intz1valid " << intersections[5].valid << endl;
}

kdTreeNode* getTrianguleList( ray_fixed* ray, kdTreeNode* curNode, kdTreeNode* tree )
{
	int i = 0;
	bool found;
	kdTreeNode* nextNode;
	pairInt intersections[6];
	point_fixed intPoint;
	getNodeInterPoints( ray, curNode, tree, intersections );
	found = false;
	for( i = 0; i < 6; i++ )
	{
		if( intersections[i].valid  == true && found == true )
		{
			//cout << "Houston, we have a problem" << endl;
			for( int j = 0; j < 6; j++ )
			{
				//cout << " valid: " << intersections[j].valid << endl;
				//cout << "x: " << intersections[j].I.x << " y " <<  intersections[j].I.y << " z " << intersections[j].I.z << endl;
			}
		}
		if( intersections[i].valid == true )
		{
			found = true;
			intPoint = intersections[i].I;
		}
	}

	//cout << "intPoint x: " << intPoint.x << " y " << intPoint.y << " z " << intPoint.z << endl;

	if( found == false ) //no intersection found
	{
		return NULL; //outside space
	}
	nextNode = traverse(tree, intPoint);

	//only for test
	ray->start = intPoint;

	return nextNode;
}

//used for stocking the points of triangles
//TODO: this is only a structure that is used for charging the triangles
//So, in the definitive implementation, It is necessary destroy-it after charging the triangles
//Due a stack_overflow_error, this vector is not inside GetTriangle
point_fixed vertex[MAXPOINTS];

int GetTriangle ( char* trigFileName, unsigned int* NbTriangles, triangle triangles[MAXTRIANGLES] )
{
	int NbPoint, NbTriangle, NbLine, Vertex1, Vertex2, Vertex3;
	char line[MAXSTRING];
	float x,y,z;
	point_fixed temp;
	FILE *fl;

	if( ( fl = fopen( trigFileName, "r" ) ) == NULL )
	{
		//printf( "file error: %s ", trigFileName );
		return 0;
	}

	NbLine=0;

	while ( fgets(line, 100, fl ) != NULL )
	{
		if( NbLine ==1 )
		{
			sscanf( line, "%d %d %*d", &NbPoint, &NbTriangle );
			//printf( "NbPoint: %d, NbTriangle %d", NbPoint, NbTriangle );
		}
		else
		{
			if( NbLine >= 2 && NbLine <= NbPoint + 1 )
			{
				sscanf(line, "%f %f %f",&x,&y,&z);
				//printf( "Pf: x %f  y: %f  z: %f \n", x, y, z );
				temp.x = (x+1)/12;
				temp.y = (y+1)/12;
				temp.z = (z+1)/12;

				vertex[NbLine-2].x = temp.x;
				vertex[NbLine-2].y = temp.y;
				vertex[NbLine-2].z = temp.z;

				//cout << "Pt: x " << vertex[NbLine-2].x << " y: " << vertex[NbLine-2].y << " z: " << vertex[NbLine-2].z << endl;
			}
			else
			{
				//cout << "Nbline " << NbLine << endl;
				if( NbLine >= NbPoint + 2 && NbLine < NbPoint + NbTriangle + 2 )
				{
					//printf("%s\n",line);
					sscanf( line, "%*d %d %d %d", &Vertex1, &Vertex2, &Vertex3 );
					triangles[ NbLine - NbPoint - 2 ].V1    = vertex[ Vertex1 ];
					triangles[ NbLine - NbPoint - 2 ].V2    = vertex[ Vertex2 ];
					triangles[ NbLine - NbPoint - 2 ].V3    = vertex[ Vertex3 ];
					triangles[ NbLine - NbPoint - 2 ].index = NbLine - NbPoint - 2;

					if( triangles[ NbLine - NbPoint - 2 ].V1.x > triangles[ NbLine - NbPoint - 2 ].V2.x )
					{
						if( triangles[ NbLine - NbPoint - 2 ].V1.x > triangles[ NbLine - NbPoint - 2 ].V3.x )
						{
							if( triangles[ NbLine - NbPoint - 2 ].V2.x > triangles[ NbLine - NbPoint - 2 ].V3.x )
							{
								//321
								triangles[ NbLine - NbPoint - 2 ].big.x = triangles[ NbLine - NbPoint - 2 ].V1.x;
								triangles[ NbLine - NbPoint - 2 ].small.x = triangles[ NbLine - NbPoint - 2 ].V3.x;
							}
							else
							{
								//231
								triangles[ NbLine - NbPoint - 2 ].big.x = triangles[ NbLine - NbPoint - 2 ].V1.x;
								triangles[ NbLine - NbPoint - 2 ].small.x = triangles[ NbLine - NbPoint - 2 ].V2.x;
							}
						}
						else
						{
							//213
							triangles[ NbLine - NbPoint - 2 ].big.x = triangles[ NbLine - NbPoint - 2 ].V3.x;
							triangles[ NbLine - NbPoint - 2 ].small.x = triangles[ NbLine - NbPoint - 2 ].V2.x;
						}
					}
					else
					{
						if( triangles[ NbLine - NbPoint - 2 ].V1.x > triangles[ NbLine - NbPoint - 2 ].V3.x)
						{
							//312
							triangles[ NbLine - NbPoint - 2 ].big.x = triangles[ NbLine - NbPoint - 2 ].V2.x;
							triangles[ NbLine - NbPoint - 2 ].small.x = triangles[ NbLine - NbPoint - 2 ].V3.x;
						}
						else
						{
							if( triangles[ NbLine - NbPoint - 2 ].V2.x > triangles[ NbLine - NbPoint - 2 ].V3.x)
							{
								//132
								triangles[ NbLine - NbPoint - 2 ].big.x = triangles[ NbLine - NbPoint - 2 ].V2.x;
								triangles[ NbLine - NbPoint - 2 ].small.x = triangles[ NbLine - NbPoint - 2 ].V1.x;
							}
							else
							{
								//123
								triangles[ NbLine - NbPoint - 2 ].big.x = triangles[ NbLine - NbPoint - 2 ].V3.x;
								triangles[ NbLine - NbPoint - 2 ].small.x = triangles[ NbLine - NbPoint - 2 ].V1.x;
							}
						}
					}

					if( triangles[ NbLine - NbPoint - 2 ].V1.y > triangles[ NbLine - NbPoint - 2 ].V2.y)
					{
						if( triangles[ NbLine - NbPoint - 2 ].V1.y > triangles[ NbLine - NbPoint - 2 ].V3.y)
						{
							if( triangles[ NbLine - NbPoint - 2 ].V2.y > triangles[ NbLine - NbPoint - 2 ].V3.y)
							{
								//321
								triangles[ NbLine - NbPoint - 2 ].big.y = triangles[ NbLine - NbPoint - 2 ].V1.y;
								triangles[ NbLine - NbPoint - 2 ].small.y = triangles[ NbLine - NbPoint - 2 ].V3.y;
							}
							else
							{
								//231
								triangles[ NbLine - NbPoint - 2 ].big.y = triangles[ NbLine - NbPoint - 2 ].V1.y;
								triangles[ NbLine - NbPoint - 2 ].small.y = triangles[ NbLine - NbPoint - 2 ].V2.y;
							}
						}
						else
						{
							//213
							triangles[ NbLine - NbPoint - 2 ].big.y = triangles[ NbLine - NbPoint - 2 ].V3.y;
							triangles[ NbLine - NbPoint - 2 ].small.y = triangles[ NbLine - NbPoint - 2 ].V2.y;
						}
					}
					else
					{
						if( triangles[ NbLine - NbPoint - 2 ].V1.y > triangles[ NbLine - NbPoint - 2 ].V3.y)
						{
							//312
							triangles[ NbLine - NbPoint - 2 ].big.y = triangles[ NbLine - NbPoint - 2 ].V2.y;
							triangles[ NbLine - NbPoint - 2 ].small.y = triangles[ NbLine - NbPoint - 2 ].V3.y;
						}
						else
						{
							if( triangles[ NbLine - NbPoint - 2 ].V2.y > triangles[ NbLine - NbPoint - 2 ].V3.y)
							{
								//132
								triangles[ NbLine - NbPoint - 2 ].big.y = triangles[ NbLine - NbPoint - 2 ].V2.y;
								triangles[ NbLine - NbPoint - 2 ].small.y = triangles[ NbLine - NbPoint - 2 ].V1.y;
							}
							else
							{
								//123
								triangles[ NbLine - NbPoint - 2 ].big.y = triangles[ NbLine - NbPoint - 2 ].V3.y;
								triangles[ NbLine - NbPoint - 2 ].small.y = triangles[ NbLine - NbPoint - 2 ].V1.y;
							}
						}
					}

					if( triangles[ NbLine - NbPoint - 2 ].V1.z > triangles[ NbLine - NbPoint - 2 ].V2.z)
					{
						if( triangles[ NbLine - NbPoint - 2 ].V1.z > triangles[ NbLine - NbPoint - 2 ].V3.z)
						{
							if( triangles[ NbLine - NbPoint - 2 ].V2.z > triangles[ NbLine - NbPoint - 2 ].V3.z)
							{
								//321
								triangles[ NbLine - NbPoint - 2 ].big.z = triangles[ NbLine - NbPoint - 2 ].V1.z;
								triangles[ NbLine - NbPoint - 2 ].small.z = triangles[ NbLine - NbPoint - 2 ].V3.z;
							}
							else
							{
								//231
								triangles[ NbLine - NbPoint - 2 ].big.z = triangles[ NbLine - NbPoint - 2 ].V1.z;
								triangles[ NbLine - NbPoint - 2 ].small.z = triangles[ NbLine - NbPoint - 2 ].V2.z;
							}
						}
						else
						{
							//213
							triangles[ NbLine - NbPoint - 2 ].big.z = triangles[ NbLine - NbPoint - 2 ].V3.z;
							triangles[ NbLine - NbPoint - 2 ].small.z = triangles[ NbLine - NbPoint - 2 ].V2.z;
						}
					}
					else
					{
						if( triangles[ NbLine - NbPoint - 2 ].V1.z > triangles[ NbLine - NbPoint - 2 ].V3.z )
						{
							//312
							triangles[ NbLine - NbPoint - 2 ].big.z = triangles[ NbLine - NbPoint - 2 ].V2.z;
							triangles[ NbLine - NbPoint - 2 ].small.z = triangles[ NbLine - NbPoint - 2 ].V3.z;
						}
						else
						{
							if( triangles[ NbLine - NbPoint - 2 ].V2.z > triangles[ NbLine - NbPoint - 2 ].V3.z )
							{
								//132
								triangles[ NbLine - NbPoint - 2 ].big.z = triangles[ NbLine - NbPoint - 2 ].V2.z;
								triangles[ NbLine - NbPoint - 2 ].small.z = triangles[ NbLine - NbPoint - 2 ].V1.z;
							}
							else
							{
								//123
								triangles[ NbLine - NbPoint - 2 ].big.z = triangles[ NbLine - NbPoint - 2 ].V3.z;
								triangles[ NbLine - NbPoint - 2 ].small.z = triangles[ NbLine - NbPoint - 2 ].V1.z;
							}
						}
					}

					//cout << "Triangle " << triangles[NbLine-NbPoint-2].index << endl;
					//cout << triangles[NbLine-NbPoint-2].index  <<"  V1: x " << triangles[NbLine-NbPoint-2].V1.x << " y "<< triangles[NbLine-NbPoint-2].V1.y << " z: " << triangles[NbLine-NbPoint-2].V1.z << endl;
					//cout << triangles[NbLine-NbPoint-2].index  <<"  V2: x " << triangles[NbLine-NbPoint-2].V2.x << " y "<< triangles[NbLine-NbPoint-2].V2.y << " z: " << triangles[NbLine-NbPoint-2].V2.z << endl;
					//cout << triangles[NbLine-NbPoint-2].index  <<"  V3: x " << triangles[NbLine-NbPoint-2].V3.x << " y "<< triangles[NbLine-NbPoint-2].V3.y << " z: " << triangles[NbLine-NbPoint-2].V3.z << endl;

					//cout<< "Triangle big   " << triangles[NbLine-NbPoint-2].big.x << " y "<< triangles[NbLine-NbPoint-2].big.y << "z: " << triangles[NbLine-NbPoint-2].big.z << endl;
					//cout<< "Triangle small " << triangles[NbLine-NbPoint-2].small.x << " y " << triangles[NbLine-NbPoint-2].small.y << " z " << triangles[NbLine-NbPoint-2].small.z << endl;
					//materialId is took from scene.dsc file
					//cout<< "MaterialId " << triangles[NbLine-NbPoint-2].materialId << endl;
				}

			}

		}
		NbLine ++;
	}

	//close files
	*NbTriangles = NbTriangle;
	fclose( fl );
	return 1;
}

bool init( char* sceneDescName, char* trigFileName, scene* myScene )
{
	float v[6];
	mfp fv[6];
	char line[MAXSTRING];
	unsigned int NbLine;
	int i;
	FILE *fdesc;

	if( ( fdesc = fopen( sceneDescName, "r" ) ) == NULL )
	{
		printf( "can't open scene description file %s \n", sceneDescName );
		return 0;
	}

	NbLine = 0;

	while ( fgets( line, 100, fdesc )!= NULL )
	{
		if( NbLine == 0 )
		{
			//printf( "first line %s", line );
		}
		else
		{
			if( NbLine == 1 )
			{
				sscanf( line, "%d %d", &myScene->sizex, &myScene->sizey );
				//printf( "sizex: %d sizey: %d\n", myScene->sizex, myScene->sizey );
			}
			else
			{
				if( NbLine == 2 )
				{
					sscanf( line, "%d %d", &myScene->NbMaterials, &myScene->NbLights );
					//printf( "NbMaterials: %d NbLights: %d\n", myScene->NbMaterials, myScene->NbLights );
				}
				else
				{
					if( NbLine > 2 && NbLine < myScene->NbMaterials + 3 )
					{
						sscanf( line, "%f %f %f %f %f %f", &v[0], &v[1], &v[2], &v[3], &v[4], &v[5] );
						for( i = 0; i < 6; i++ )
						{
							fv[i] = v[i];
							//cout << "fv[" << i << "] = " << fv[i] << endl;
						}

						myScene->materials[ NbLine - 3 ].red   			= fv[0];
						myScene->materials[ NbLine - 3 ].green 			= fv[1];
						myScene->materials[ NbLine - 3 ].blue  			= fv[2];
						myScene->materials[ NbLine - 3 ].reflection 	= fv[3];
						myScene->materials[ NbLine - 3 ].transmission 	= fv[4];
						myScene->materials[ NbLine - 3 ].nrefraction 	= fv[5];

						// cout << "M: red " << myScene->materials[ NbLine - 3 ].red << " green: " << myScene->materials[ NbLine - 3 ].green << " blue: " << myScene->materials[ NbLine - 3 ].blue << " reflection: " << myScene->materials[ NbLine - 3 ].reflection << " transmission: " << myScene->materials[ NbLine - 3 ].transmission << " refraction: " << myScene->materials[ NbLine - 3 ].nrefraction << endl;
					}
					else
					{
						if( NbLine > 2 + myScene->NbMaterials && NbLine < myScene->NbMaterials + myScene->NbLights + 3 )
						{
							sscanf( line, "%f %f %f %f %f %f", &v[0], &v[1], &v[2], &v[3], &v[4], &v[5] );
							for( i = 0; i < 6; i++ )
							{
								fv[i] = v[i];
								//cout << "fv[" << i << "] = " << fv[i] << endl;
							}

							myScene->lights[ NbLine - ( 3 + myScene->NbMaterials ) ].pos.x   	= fv[0];
							myScene->lights[ NbLine - ( 3 + myScene->NbMaterials ) ].pos.y 		= fv[1];
							myScene->lights[ NbLine - ( 3 + myScene->NbMaterials ) ].pos.z  	= fv[2];
							myScene->lights[ NbLine - ( 3 + myScene->NbMaterials ) ].red 		= fv[3];
							myScene->lights[ NbLine - ( 3 + myScene->NbMaterials ) ].green 		= fv[4];
							myScene->lights[ NbLine - ( 3 + myScene->NbMaterials ) ].blue 		= fv[5];

							//cout << "L: x " << myScene->lights[ NbLine - ( 3 + myScene->NbMaterials ) ].pos.x << " y: " << myScene->lights[ NbLine - ( 3 + myScene->NbMaterials ) ].pos.y << " z: " << myScene->lights[ NbLine - ( 3 + myScene->NbMaterials ) ].pos.z << " red: " << myScene->lights[ NbLine - ( 3 + myScene->NbMaterials ) ].red << " green: " << myScene->lights[ NbLine - ( 3 + myScene->NbMaterials ) ].green << " blue: " << myScene->lights[ NbLine - ( 3 + myScene->NbMaterials ) ].blue << endl;
						}
						else

						{
							if( NbLine == myScene->NbMaterials + myScene->NbLights + 3 )
							{
								sscanf( line, "%d", &myScene->NbTriangles );
								//printf( "NbTriangles: %d \n", myScene->NbTriangles );
							}
							else
							{
								if( NbLine > myScene->NbMaterials + myScene->NbLights + 3 && NbLine < myScene->NbMaterials + myScene->NbLights + myScene->NbTriangles + 4 )
								{
									sscanf( line, "%d", &myScene->triangles[ NbLine - ( myScene->NbMaterials + myScene->NbLights + 4 ) ].materialId );
									//printf( "triangle[%d] materialId: %d\n", NbLine - ( myScene->NbMaterials + myScene->NbLights + 4 ), myScene->triangles[ NbLine - ( myScene->NbMaterials + myScene->NbLights + 4 ) ].materialId );
								}
							}
						}
					}
				}
			}
		}
		NbLine++;
	}
	fclose( fdesc );

	//read triangles
	//printf( "reading triangles\n" );
	int TrianglesExist = GetTriangle ( trigFileName, &( myScene->NbTriangles ), myScene->triangles );
	if ( TrianglesExist == 0 )
	{
		//printf( "Scene file : Bad Triangle section\n" );
	    return false;
	}

	return true;
}
