#include "Scene.h"

#define IS_NULL(x) (((-EPSILON) < (x)) && ((x) < EPSILON))
#define IS_OUT(x)  (((x) > 3.5) || (x) < -3.5)

//helper method - returns squared distance to the node's bounds (0 if inside)
ifixed disq( kdTreeNode* ctr, point_fixed pos)
{
	const coord & x = pos.x;
	const coord & y = pos.y;
	const coord & z = pos.z;
	const coord & x0 = ctr->x0;
	const coord & y0 = ctr->y0;
	const coord & x1 = ctr->x1;
	const coord & y1 = ctr->y1;
	const coord & z0 = ctr->z0;
	const coord & z1 = ctr->z1;

	if(x>x0&&y>y0&&x<x1&&y<y1&&z>z0&&z<z1) return 0.0;
	coord dx = 0.0;
	coord dy = 0.0;
	coord dz = 0.0;
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
void insertSplitPos( SplitList** sList, SplitList** splitPool, coord splitPos )
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
			// cout << "splitpos " << splitPos
			//      << " comp " << p->splitpos
			//      << " inserted lowest case" << endl;
		}
		else if ( splitPos == p->splitpos )
		{
			entry->next = *splitPool; // redundant; recycle
			*splitPool = entry;
			// cout << "splitpos " << splitPos
			//      << " comp " << p->splitpos
			//      <<  " redundant equal case" << endl;
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
					// cout << "splitpos " << splitPos
					//      << " comp " << p->next->splitpos
					//      << " redundant equal iterative case " << endl;
					return;
				}
				p = p->next;
			}
			entry->next = p->next;
			p->next = entry;
			// coutdeb << "splitpos " << splitPos
			//         << " comp " << p->splitpos
			//         << " inserted iterative case " << endl;
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

bool build( TrigRefList** trianglelist, TrigRefList** freeRefNodes,
            kdTreeNode** freeNodes, kdTreeNode * tree, const int depth )
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

	do
	{
		do
		{
			if( cdepth == depth || ctrAnt->pnum <= 2 )
			{
				printfdeb( "Max Depth or few nodes stopping condition\n");
				printfdeb( "put %d triangles in the tree\n", ctrAnt->pnum );

				// cout << "index of first triangle: " << ctrAnt->trig->ref->index
				//      << endl;
				coutdeb << "number of triangles: " << ctrAnt->pnum << endl;

				// cout << "null sons " << ctrAnt->left
				//      << "    " << ctrAnt->right << endl;
				//print( "leaf iteration done\n\n" );
			}
			else
			{
				printfdeb( "building new nodes. Number of triangles: %d\n",
				           ctrAnt->pnum );

				int axis;
				//splits along the axis of who the tree's bound is largest
				if ( ( ctrAnt->x1 - ctrAnt->x0 > ctrAnt->y1 - ctrAnt->y0 )
				     && ( ctrAnt->x1 - ctrAnt->x0 > ctrAnt->z1 - ctrAnt->z0 ) )
				{
					axis = 0;
					l1 = ctrAnt->x0; l2 = ctrAnt->x1;
				}
				else if ( ( ctrAnt->y1 - ctrAnt->y0 > ctrAnt->z1 - ctrAnt->z0 )
				          && ( ctrAnt->y1 - ctrAnt->y0
				               > ctrAnt->z1 - ctrAnt->z0 ) )
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
				printfdeb( "split axis: %d\n", ctrAnt->spx );

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
				coutdeb << "l1 " << l1 << " l2 " << l2 << endl;
				while ( aidx < ctrAnt->pnum )
				{
					if( axis == 0 )
					{
						if( l->ref->V1.x > l1 && l->ref->V1.x < l2 )
							insertSplitPos( &sList, &sPool, l->ref->V1.x );
						if( l->ref->V2.x > l1 && l->ref->V2.x < l2 )
							insertSplitPos( &sList, &sPool, l->ref->V2.x );
						if( l->ref->V3.x > l1 && l->ref->V3.x < l2 )
							insertSplitPos( &sList, &sPool, l->ref->V3.x );
					}
					else if ( axis == 1 )
					{
						if( l->ref->V1.y > l1 && l->ref->V1.y < l2 )
							insertSplitPos( &sList, &sPool, l->ref->V1.y );
						if( l->ref->V2.y > l1 && l->ref->V2.y < l2 )
							insertSplitPos( &sList, &sPool, l->ref->V2.y );
						if( l->ref->V3.y > l1 && l->ref->V3.y < l2 )
							insertSplitPos( &sList, &sPool, l->ref->V3.y );
					}
					else
					{
						if( l->ref->V1.z > l1 && l->ref->V1.z < l2 )
							insertSplitPos( &sList, &sPool, l->ref->V1.z );
						if( l->ref->V2.z > l1 && l->ref->V2.z < l2 )
							insertSplitPos( &sList, &sPool, l->ref->V2.z );
						if( l->ref->V3.z > l1 && l->ref->V3.z < l2 )
							insertSplitPos( &sList, &sPool, l->ref->V3.z );
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

						ltrigmin = (( axis == 0 ) ? t->ref->small.x
						            : ( axis == 1 ) ? t->ref->small.y
						            : t->ref->small.z);
						ltrigmax = (( axis == 0 ) ? t->ref->big.x
						            : ( axis == 1 ) ? t->ref->big.y
						            : t->ref->big.z);
						//printf ( "\n" );
						//printf ( "trig %d\n", t->ref->index );
						//cout << "l1 " << l1 << " l2 " << l2 << endl;
						//cout << "l3 " << l3 << " l4 " << l4 << endl;
						// cout << "ltrigmin: " << ltrigmin
						//      << " ltrigmax: " << ltrigmax << endl;
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
				surfAreaTot SAV = 0.5 / ( ( ctrAnt->x1 - ctrAnt->x0 )
				                          * ( ctrAnt->y1 - ctrAnt->y0 )
				                          + ( ctrAnt->y1 - ctrAnt->y0 )
				                          * ( ctrAnt->z1 - ctrAnt->z0 )
				                          + ( ctrAnt->x1 - ctrAnt->x0 )
				                          * ( ctrAnt->z1 - ctrAnt->z0 ) );
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

					//coutdeb << "splitpos " << splist->splitpos << endl;
					// coutdeb << "n1count " << splist->n1count
					//         << " n2count " << splist->n2count << endl;
					// coutdeb << "b3[0] " << b3[0]
					//         << " b3[1] " << b3[1]
					//         << " b3[2] " << b3[2]
					//         << " b3[3] " << b3[3]
					//         << " b3[4] " << b3[4]
					//         <<  " b3[5] " << b3[5] << endl;
					// coutdeb << "b4[0] " << b4[0]
					//         << " b4[1] " << b4[1]
					//         << " b4[2] " << b4[2]
					//         << " b4[3] " << b4[3]
					//         << " b4[4] " << b4[4]
					//         <<  " b4[5] " << b4[5] << endl;

					// calculate child node cost
					surfArea SA1 = 2 * (( b3[3] - b3[0] )
					                    * ( b3[4] - b3[1] )
					                    + ( b3[3] - b3[0] )
					                    * ( b3[5] - b3[2] )
					                    + ( b3[4] - b3[1] )
					                    * ( b3[5] - b3[2] ) );
					surfArea SA2 = 2 * (( b4[3] - b4[0] ) * ( b4[4] - b4[1] )
					                    + ( b3[3] - b3[0] ) * ( b4[5] - b4[2] )
					                    + ( b4[4] - b4[1] )
					                    * ( b4[5] - b4[2] ) );
					//coutdeb << "SA1 " << SA1 << " SA2 " << SA2 << endl;

					cost splitcost = (0.3 + 1.0
					                  * (SA1 * SAV * splist->n1count
					                     + SA2 * SAV * splist->n2count));
					//coutdeb << lowcost;
					// coutdeb << "split cost! " << splitcost
					//         << "low cost" << lowcost << endl;
					// update best cost tracking variables
					if (splitcost < lowcost)
					{
						lowcost = splitcost;
						bestpos = splist->splitpos;
						b1[axis + 3] = bestpos;
						b2[axis] = bestpos;
						bestn1count = splist->n1count;
						bestn2count = splist->n2count; //best division elements
					}
					splist = splist->next;
				}
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
					for ( int i = 0; i < ctrAnt->pnum; i++ )
					{
						coord ltrigmin, ltrigmax;
						ltrigmin = (( axis == 0 ) ? l->ref->small.x
						            : ( axis == 1 ) ? l->ref->small.y
						            : l->ref->small.z);
						ltrigmax = (( axis == 0 ) ? l->ref->big.x
						            : ( axis == 1 ) ? l->ref->big.y
						            : l->ref->big.z);

						if( DEBUG )
						{
							printf( "trig index %d\n", l->ref->index );
							cout << "ltrigmin " << ltrigmin
							     << " ltrigmax " << ltrigmax <<endl;
							cout << "l1 " << l1
							     << " l2 " << l2 << endl;
							cout << "l3 " << l3
							     << " l4 " << l4 << endl;
						}
						if ( (ltrigmin <= l2) && ( ltrigmax >= l1))
						{
							if( insersectBox( b1, l->ref ) )
							{
								tleft[n1count] = l->ref;
								n1count++;
								coutdeb << "It has been added to left" << endl;
							}
						}
						if ((ltrigmin <= l4) && (ltrigmax >= l3))
						{
							if( insersectBox( b2, l->ref ) )
							{
								tright[n2count] = l->ref;
								n2count++;
								coutdeb << "It has been added to right" << endl;
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
					printfdeb("\n\n new nodes:\n");
					printfdeb( "split axis: %d\n\n", ctrAnt->spx );

					ctrAnt->left = newNode( freeNodes,
					                        b1[0],
					                        b1[1],
					                        b1[2],
					                        b1[3],
					                        b1[4],
					                        b1[5] );
					ctrAnt->right = newNode( freeNodes,
					                         b2[0],
					                         b2[1],
					                         b2[2],
					                         b2[3],
					                         b2[4],
					                         b2[5] );

					if( ctrAnt->left == NULL || ctrAnt->right == NULL )
					{
						printf("build - No free Node\n");
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
							printf( "build - no disponible refs "\
							        "for left node\n" );
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
							printf( "build - no disponible refs "\
							        "for left node\n" );
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
							printf( "build - no disponible refs"\
							        "for right node\n" );
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
							printf( "build - no disponible refs" \
							        "for right node\n" );
							break;
						}
					}
					printfdeb( "next nodes elements; left: %d right: %d\n",
					           ctrAnt->left->pnum, ctrAnt->right->pnum );

					ctrAnt->left->next = ctrAnt->right;

					if( ctrBegin == NULL )
					{
						//printf( "first new level node\n" );
						ctrBegin = ctrAnt->left;
					}
					else
					{
						//printf( "bounding previous new level node\n" );
						ctr->next = ctrAnt->left;
					}
					//printfdeb( "pendent bound\n " );
					ctr = ctrAnt->right;
					if( DEBUG )
					{
						printf( "new sub nodes:\n" );
						printf( "numl : %d numr: %d cdepth: %d\n",
						        ctrAnt->left->pnum, ctrAnt->right->pnum,
						        cdepth );
						printf( "build iteration done!\n\n" );
					}
				}
				else
				{
					printfdeb( "NOT WORTHY SPLITTING\n" );
				}
			}
			ctrAnt = ctrAnt->next;
			if( ctrAnt != NULL )
			{
				printfdeb( "next one: %d\n", ctrAnt->pnum );
			}
		} while (ctrAnt != NULL);
		if(ctr)
		{
			ctr->next = NULL;
			//printfdeb( "pendent bound = NULL\n");
		}

		printfdeb( "going to next tree level\n" );
		printfdeb( "printing next level elements:\n" );

		ctrAnt = ctrBegin;
		while ( ctrAnt != NULL )
		{
			printfdeb( "pnum: %d\n", ctrAnt->pnum );
			ctrAnt = ctrAnt->next;
		}

		ctrAnt = ctrBegin;
		ctrBegin = NULL;
		ctr = NULL;
		cdepth++;
	} while ( ctrAnt != NULL );
	return true;
}

//kd-tree traversal
kdTreeNode * traverse(kdTreeNode * ctr, const point_fixed  pos)
{
	if( ctr == NULL ) return NULL;
	while( ctr->trig == NULL )
	{
		if( ctr->spx == 0 )
		{
			coutdeb << "traversing x axis: " << ctr->left->x1 << endl;
			if( pos.x < ctr->left->x1 )
			{
				coutdeb << "left" << endl;
				ctr = ctr->left;
			}
			else
			{
				coutdeb << "right" << endl;
				ctr = ctr->right;
			}
		}
		else if( ctr->spx == 1 )
		{
			coutdeb << "traversing y axis: " << ctr->left->y1 << endl;
			if( pos.y < ctr->left->y1 )
			{
				coutdeb << "left" << endl;
				ctr = ctr->left;
			}
			else
			{
				coutdeb << "right" << endl;
				ctr = ctr->right;
			}
		}
		else
		{
			coutdeb << "traversing z axis: " << ctr->left->z1 << endl;
			if( pos.z < ctr->left->z1 )
			{
				coutdeb << "left" << endl;
				ctr = ctr->left;
			}
			else
			{
				coutdeb << "right" << endl;
				ctr = ctr->right;
			}
		}
	}
	return ctr;
}

int getKdNodeIntersections( ray* ray, kdTreeNode* i, pairInt intersections[6] )
{
	//normal vectors
	vect_fixed nx = { 1, 0, 0 };
	vect_fixed ny = { 0, 1, 0 };
	vect_fixed nz = { 0, 0, 1 };

	coutdeb << "i->x0: " << i->x0
	        << " i->x1 " << i->x1
	        << " i->y0 " << i->y0
	        << " i->y1 " << i->y1
	        << " i->z0 " << i->z0
	        << " i->z1 " << i->z1 << endl;
	//plan points
	point_fixed vx0 = { i->x0, 0 ,0 };
	point_fixed vx1 = { i->x1, 0 ,0 };
	point_fixed vy0 = { 0, i->y0, 0 };
	point_fixed vy1 = { 0, i->y1, 0 };
	point_fixed vz0 = { 0, 0, i->z0 };
	point_fixed vz1 = { 0, 0, i->z1 };

	coutdeb << "ray start x " << ray->start.x
	        << " y " << ray->start.y
	        << " z " << ray->start.z << endl;
	coutdeb << "ray dir x " << ray->dir.x
	        << " y " << ray->dir.y
	        << " z " << ray->dir.z << endl;

	vect_fixed wx0, wx1, wy0, wy1, wz0, wz1;
	sfv Nx0, Nx1, Ny0, Ny1, Nz0, Nz1;
	divp sx0, sx1, sy0, sy1, sz0, sz1;

	for (int i = 0; i < 6; i++)
		intersections[i].valid = false;

	sfv Dx = nx * ray->dir;
	//plan parallel ray

	if( IS_NULL(Dx) ) {
		//Dx > 1 == outside the space because max NX0 = 3
		// and max Sx0 = sqrt(2^2 + 2^2 + 2^2)
		intersections[0].I.x = 0;
		intersections[0].I.y = 0;
		intersections[0].I.z = 0;
		intersections[1].I.x = 0;
		intersections[1].I.y = 0;
		intersections[1].I.z = 0;
	}
	else
	{
		wx0 = ray->start - vx0;
		Nx0 = - ( nx * wx0 );
		sx0 = Nx0 / Dx;
		wx1 = ray->start - vx1;
		Nx1 = - ( nx * wx1 );
		sx1 = Nx1 / Dx;

		if( IS_OUT(sx0) || IS_NULL(sx0) )
			//outside the space limits (cube from -1 to 1 in each axis).
			// Max sx0 = sqrt(12)
			//or its the point that is already calculated (ray.start)
		{
			intersections[0].I.x = 0;
			intersections[0].I.y = 0;
			intersections[0].I.z = 0;
		}
		else
		{
			intersections[0].I = ray->start + ( sx0 * ray->dir );
			if(intersections[0].I.x >= i->x0 && intersections[0].I.y >= i->y0
			   && intersections[0].I.z >= i->z0 && intersections[0].I.x <= i->x1
			   && intersections[0].I.y <= i->y1
			   && intersections[0].I.z <= i->z1)
			{
				//it is inside the region that is being treated
				intersections[0].valid = true;
			}
		}

		if( IS_OUT(sx1) || IS_NULL(sx1) )
		{
			intersections[1].I.x = 0;
			intersections[1].I.y = 0;
			intersections[1].I.z = 0;
		}
		else
		{
			intersections[1].I = ray->start + ( sx1 * ray->dir );
			if(intersections[1].I.x >= i->x0 && intersections[1].I.y >= i->y0
			   && intersections[1].I.z >= i->z0 && intersections[1].I.x <= i->x1
			   && intersections[1].I.y <= i->y1
			   && intersections[1].I.z <= i->z1)
			{
				intersections[1].valid = true;
			}
		}
	}
	sfv Dy = ny * ray->dir;
	if( IS_NULL(Dy) )
	{
		intersections[2].I.x = 0;
		intersections[2].I.y = 0;
		intersections[2].I.z = 0;
		intersections[3].I.x = 0;
		intersections[3].I.y = 0;
		intersections[3].I.z = 0;

	}
	else
	{
		wy0 = ray->start - vy0;
		Ny0 = - ( ny * wy0 );
		sy0 = Ny0 / Dy;
		wy1 = ray->start - vy1;
		Ny1 = - ( ny * wy1 );
		sy1 = Ny1 / Dy;

		if( IS_OUT(sy0) || IS_NULL(sy0) )
		{
			intersections[2].I.x = 0;
			intersections[2].I.y = 0;
			intersections[2].I.z = 0;
		}
		else
		{
			intersections[2].I = ray->start + ( sy0 * ray->dir );
			if( intersections[2].I.x >= i->x0 && intersections[2].I.y >= i->y0
			    && intersections[2].I.z >= i->z0
			    && intersections[2].I.x <= i->x1
			    && intersections[2].I.y <= i->y1
			    && intersections[2].I.z <= i->z1)
			{
				intersections[2].valid = true;
			}
		}

		if( IS_OUT(sy1) || IS_NULL(sy1) )
		{
			intersections[3].I.x = 0;
			intersections[3].I.y = 0;
			intersections[3].I.z = 0;
		}
		else
		{
			intersections[3].I = ray->start + ( sy1 * ray->dir );
			if( intersections[3].I.x >= i->x0 && intersections[3].I.y >= i->y0
			    && intersections[3].I.z >= i->z0
			    && intersections[3].I.x <= i->x1
			    && intersections[3].I.y <= i->y1
			    && intersections[3].I.z <= i->z1)
			{
				intersections[3].valid = true;
			}
		}
	}
	sfv Dz = nz * ray->dir;
	if( IS_NULL(Dz) ) {
		intersections[4].I.x = 0;
		intersections[4].I.y = 0;
		intersections[4].I.z = 0;
		intersections[5].I.x = 0;
		intersections[5].I.y = 0;
		intersections[5].I.z = 0;
	}
	else
	{
		wz0 = ray->start - vz0;
		Nz0 = - ( nz * wz0 );
		sz0 = Nz0 / Dz;
		wz1 = ray->start - vz1;
		Nz1 = - ( nz * wz1 );
		sz1 = Nz1 / Dz;

		if( IS_OUT(sz0) || IS_NULL(sz0) )
		{
			intersections[4].I.x = 0;
			intersections[4].I.y = 0;
			intersections[4].I.z = 0;
		}
		else
		{
			intersections[4].I = ray->start + ( sz0 * ray->dir );
			if(intersections[4].I.x >=i->x0 && intersections[4].I.y >= i->y0
			   && intersections[4].I.z >= i->z0
			   && intersections[0].I.x <= i->x1
			   && intersections[4].I.y < i->y1
			   && intersections[4].I.z <= i->z1)
			{
				intersections[4].valid = true;
			}
		}

		if( IS_OUT(sz1) || IS_NULL(sz1) )
		{
			intersections[5].I.x = 0;
			intersections[5].I.y = 0;
			intersections[5].I.z = 0;
		}
		else
		{
			intersections[5].I = ray->start + ( sz1 * ray->dir );
			if(intersections[5].I.x >=i->x0 && intersections[5].I.y >= i->y0
			   && intersections[5].I.z >= i->z0
			   && intersections[1].I.x <= i->x1
			   && intersections[5].I.y <= i->y1
			   && intersections[5].I.z <= i->z1)
			{
				intersections[5].valid = true;
			}
		}
	}

	//verifying if there are equal intersection points (edges of the box
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

	if ( DEBUG )
	{
		cout << "Dx " << Dx << " Dy " << Dy << " Dz " << Dz << endl;

		//if zero ray inside plan
		//to do: treat this case

		cout << "Nx0 " << Nx0 <<" Nx1 " << Nx1 << " Ny0 "
		     << Ny0 << " Ny1 " << Ny1 << " Nz0 " << Nz0
		     << " Nz1 " << Nz1 << endl;
		cout << "sx0 " << sx0 << " sx1 " << sx1 << " sy0 "
		     << sy0 << " sy1 " << sy1 << " sz0 " << sz0
		     << " sz1 " << sz1 << endl;

		cout << "int x0 x " << intersections[0].I.x
		     << " y " <<  intersections[0].I.y
		     << " z " << intersections[0].I.z << endl;
		cout << "intx0valid " << intersections[0].valid << endl;

		cout << "int x1 x " << intersections[1].I.x
		     << " y " <<  intersections[1].I.y
		     << " z " << intersections[1].I.z << endl;
		cout << "intx1valid " << intersections[1].valid << endl;

		cout << "int y0 x " << intersections[2].I.x
		     << " y " <<  intersections[2].I.y
		     << " z " << intersections[2].I.z << endl;
		cout << "inty0valid " << intersections[2].valid << endl;

		cout << "int y1 x " << intersections[3].I.x
		     << " y " <<  intersections[3].I.y
		     << " z " << intersections[3].I.z << endl;
		cout << "inty1valid " << intersections[3].valid << endl;

		cout << "int z0 x " << intersections[4].I.x
		     << " y " <<  intersections[4].I.y
		     << " z " << intersections[4].I.z << endl;
		cout << "intz0valid " << intersections[4].valid << endl;

		cout << "int z1 x " << intersections[5].I.x
		     << " y " <<  intersections[5].I.y
		     << " z " << intersections[5].I.z << endl;
		cout << "intz1valid " << intersections[5].valid << endl;
	}
	return 0;
}

kdTreeNode* getTrianguleList( ray* ray, kdTreeNode* curNode, kdTreeNode* tree )
{
	int i = 0;
	bool found;
	kdTreeNode* nextNode;
	pairInt intersections[6];
	point_fixed intPoint;
	getKdNodeIntersections( ray, curNode, intersections );
	found = false;
	for( i = 0; i < 6; i++ )
	{
		if( intersections[i].valid  == true && found == true )
		{
			coutdeb << "Houston, we have a problem" << endl;
			for( int j = 0; j < 6; j++ )
			{
				coutdeb << " valid: " << intersections[j].valid << endl;
				coutdeb << "x: " << intersections[j].I.x
				        << " y " <<  intersections[j].I.y
				        << " z " << intersections[j].I.z << endl;
			}
		}
		if( intersections[i].valid == true )
		{
			found = true;
			intPoint = intersections[i].I;
		}
	}

	coutdeb << "intPoint x: " << intPoint.x
	        << " y " << intPoint.y
	        << " z " << intPoint.z << endl;

	if( found == false ) //no intersection found
	{
		return NULL; //outside space
	}
	nextNode = traverse(tree, intPoint);

	//only for test
	ray->start = intPoint;

	return nextNode;
}

int getTriangle ( char* trigFileName, unsigned int* NbTriangles,
                  triangle triangles[MAXTRIANGLES] )
{
	int NbPoint, NbTriangle, NbLine, Vertex1, Vertex2, Vertex3;
	char line[MAXSTRING];
	float x,y,z;
	point_fixed temp;
	FILE *fl;

	if( ( fl = fopen( trigFileName, "r" ) ) == NULL )
	{
		printfdeb( "file error: %s ", trigFileName );
		return 0;
	}

	NbLine=0;

	while ( fgets(line, 100, fl ) != NULL )
	{
		if( NbLine ==1 )
		{
			sscanf( line, "%d %d %*d", &NbPoint, &NbTriangle );
			printfdeb( "NbPoint: %d, NbTriangle %d", NbPoint, NbTriangle );
		}
		else
		{
			if( NbLine >= 2 && NbLine <= NbPoint + 1 )
			{
				sscanf(line, "%f %f %f",&x,&y,&z);
				//printfdeb( "Pf: x %f  y: %f  z: %f \n", x, y, z );
				temp.x = (x + 1 )/12;
				temp.y = (y + 1)/12;
				temp.z = (z + 1)/12;

				vertex[NbLine-2].x = temp.x;
				vertex[NbLine-2].y = temp.y;
				vertex[NbLine-2].z = temp.z;

				coutdeb << "Pt: x " << vertex[NbLine-2].x
				        << " y: " << vertex[NbLine-2].y
				        << " z: " << vertex[NbLine-2].z << endl;
			}
			else
			{
				coutdeb << "Nbline " << NbLine << endl;
				if( NbLine >= NbPoint + 2 && NbLine < NbPoint + NbTriangle + 2 )
				{
					//printfdeb("%s\n",line);
					sscanf( line, "%*d %d %d %d", &Vertex1, &Vertex2,
					        &Vertex3 );
					triangles[ NbLine - NbPoint - 2 ].V1    = vertex[ Vertex1 ];
					triangles[ NbLine - NbPoint - 2 ].V2    = vertex[ Vertex2 ];
					triangles[ NbLine - NbPoint - 2 ].V3    = vertex[ Vertex3 ];
					triangles[ NbLine - NbPoint - 2 ].index = (NbLine
					                                           - NbPoint - 2);

					if( triangles[ NbLine - NbPoint - 2 ].V1.x
					    > triangles[ NbLine - NbPoint - 2 ].V2.x )
					{
						if( triangles[ NbLine - NbPoint - 2 ].V1.x
						    > triangles[ NbLine - NbPoint - 2 ].V3.x )
						{
							if( triangles[ NbLine - NbPoint - 2 ].V2.x
							    > triangles[ NbLine - NbPoint - 2 ].V3.x )
							{
								//321
								triangles[ NbLine - NbPoint - 2 ].big.x =
									triangles[ NbLine - NbPoint - 2 ].V1.x;
								triangles[ NbLine - NbPoint - 2 ].small.x =
									triangles[ NbLine - NbPoint - 2 ].V3.x;
							}
							else
							{
								//231
								triangles[ NbLine - NbPoint - 2 ].big.x =
									triangles[ NbLine - NbPoint - 2 ].V1.x;
								triangles[ NbLine - NbPoint - 2 ].small.x =
									triangles[ NbLine - NbPoint - 2 ].V2.x;
							}
						}
						else
						{
							//213
							triangles[ NbLine - NbPoint - 2 ].big.x =
								triangles[ NbLine - NbPoint - 2 ].V3.x;
							triangles[ NbLine - NbPoint - 2 ].small.x =
								triangles[ NbLine - NbPoint - 2 ].V2.x;
						}
					}
					else
					{
						if( triangles[ NbLine - NbPoint - 2 ].V1.x
						    > triangles[ NbLine - NbPoint - 2 ].V3.x)
						{
							//312
							triangles[ NbLine - NbPoint - 2 ].big.x =
								triangles[ NbLine - NbPoint - 2 ].V2.x;
							triangles[ NbLine - NbPoint - 2 ].small.x =
								triangles[ NbLine - NbPoint - 2 ].V3.x;
						}
						else
						{
							if( triangles[ NbLine - NbPoint - 2 ].V2.x
							    > triangles[ NbLine - NbPoint - 2 ].V3.x)
							{
								//132
								triangles[ NbLine - NbPoint - 2 ].big.x =
									triangles[ NbLine - NbPoint - 2 ].V2.x;
								triangles[ NbLine - NbPoint - 2 ].small.x =
									triangles[ NbLine - NbPoint - 2 ].V1.x;
							}
							else
							{
								//123
								triangles[ NbLine - NbPoint - 2 ].big.x =
									triangles[ NbLine - NbPoint - 2 ].V3.x;
								triangles[ NbLine - NbPoint - 2 ].small.x =
									triangles[ NbLine - NbPoint - 2 ].V1.x;
							}
						}
					}

					if( triangles[ NbLine - NbPoint - 2 ].V1.y
					    > triangles[ NbLine - NbPoint - 2 ].V2.y)
					{
						if( triangles[ NbLine - NbPoint - 2 ].V1.y
						    > triangles[ NbLine - NbPoint - 2 ].V3.y)
						{
							if( triangles[ NbLine - NbPoint - 2 ].V2.y
							    > triangles[ NbLine - NbPoint - 2 ].V3.y)
							{
								//321
								triangles[ NbLine - NbPoint - 2 ].big.y =
									triangles[ NbLine - NbPoint - 2 ].V1.y;
								triangles[ NbLine - NbPoint - 2 ].small.y =
									triangles[ NbLine - NbPoint - 2 ].V3.y;
							}
							else
							{
								//231
								triangles[ NbLine - NbPoint - 2 ].big.y =
									triangles[ NbLine - NbPoint - 2 ].V1.y;
								triangles[ NbLine - NbPoint - 2 ].small.y =
									triangles[ NbLine - NbPoint - 2 ].V2.y;
							}
						}
						else
						{
							//213
							triangles[ NbLine - NbPoint - 2 ].big.y =
								triangles[ NbLine - NbPoint - 2 ].V3.y;
							triangles[ NbLine - NbPoint - 2 ].small.y =
								triangles[ NbLine - NbPoint - 2 ].V2.y;
						}
					}
					else
					{
						if( triangles[ NbLine - NbPoint - 2 ].V1.y
						    > triangles[ NbLine - NbPoint - 2 ].V3.y)
						{
							//312
							triangles[ NbLine - NbPoint - 2 ].big.y =
								triangles[ NbLine - NbPoint - 2 ].V2.y;
							triangles[ NbLine - NbPoint - 2 ].small.y =
								triangles[ NbLine - NbPoint - 2 ].V3.y;
						}
						else
						{
							if( triangles[ NbLine - NbPoint - 2 ].V2.y
							    > triangles[ NbLine - NbPoint - 2 ].V3.y)
							{
								//132
								triangles[ NbLine - NbPoint - 2 ].big.y =
									triangles[ NbLine - NbPoint - 2 ].V2.y;
								triangles[ NbLine - NbPoint - 2 ].small.y =
									triangles[ NbLine - NbPoint - 2 ].V1.y;
							}
							else
							{
								//123
								triangles[ NbLine - NbPoint - 2 ].big.y =
									triangles[ NbLine - NbPoint - 2 ].V3.y;
								triangles[ NbLine - NbPoint - 2 ].small.y =
									triangles[ NbLine - NbPoint - 2 ].V1.y;
							}
						}
					}

					if( triangles[ NbLine - NbPoint - 2 ].V1.z
					    > triangles[ NbLine - NbPoint - 2 ].V2.z)
					{
						if( triangles[ NbLine - NbPoint - 2 ].V1.z
						    > triangles[ NbLine - NbPoint - 2 ].V3.z)
						{
							if( triangles[ NbLine - NbPoint - 2 ].V2.z
							    > triangles[ NbLine - NbPoint - 2 ].V3.z)
							{
								//321
								triangles[ NbLine - NbPoint - 2 ].big.z =
									triangles[ NbLine - NbPoint - 2 ].V1.z;
								triangles[ NbLine - NbPoint - 2 ].small.z =
									triangles[ NbLine - NbPoint - 2 ].V3.z;
							}
							else
							{
								//231
								triangles[ NbLine - NbPoint - 2 ].big.z =
									triangles[ NbLine - NbPoint - 2 ].V1.z;
								triangles[ NbLine - NbPoint - 2 ].small.z =
									triangles[ NbLine - NbPoint - 2 ].V2.z;
							}
						}
						else
						{
							//213
							triangles[ NbLine - NbPoint - 2 ].big.z =
								triangles[ NbLine - NbPoint - 2 ].V3.z;
							triangles[ NbLine - NbPoint - 2 ].small.z =
								triangles[ NbLine - NbPoint - 2 ].V2.z;
						}
					}
					else
					{
						if( triangles[ NbLine - NbPoint - 2 ].V1.z
						    > triangles[ NbLine - NbPoint - 2 ].V3.z )
						{
							//312
							triangles[ NbLine - NbPoint - 2 ].big.z =
								triangles[ NbLine - NbPoint - 2 ].V2.z;
							triangles[ NbLine - NbPoint - 2 ].small.z =
								triangles[ NbLine - NbPoint - 2 ].V3.z;
						}
						else
						{
							if( triangles[ NbLine - NbPoint - 2 ].V2.z >
							    triangles[ NbLine - NbPoint - 2 ].V3.z )
							{
								//132
								triangles[ NbLine - NbPoint - 2 ].big.z =
									triangles[ NbLine - NbPoint - 2 ].V2.z;
								triangles[ NbLine - NbPoint - 2 ].small.z =
									triangles[ NbLine - NbPoint - 2 ].V1.z;
							}
							else
							{
								//123
								triangles[ NbLine - NbPoint - 2 ].big.z =
									triangles[ NbLine - NbPoint - 2 ].V3.z;
								triangles[ NbLine - NbPoint - 2 ].small.z =
									triangles[ NbLine - NbPoint - 2 ].V1.z;
							}
						}
					}

					if ( DEBUG )
					{
						cout << "Triangle "
						     << triangles[NbLine-NbPoint-2].index << endl;
						cout << triangles[NbLine-NbPoint-2].index
						     <<"  V1: x "<< triangles[NbLine-NbPoint-2].V1.x
						     << " y "<< triangles[NbLine-NbPoint-2].V1.y
						     << " z: " << triangles[NbLine-NbPoint-2].V1.z
						     << endl;
						cout << triangles[NbLine-NbPoint-2].index
						     <<"  V2: x " << triangles[NbLine-NbPoint-2].V2.x
						     << " y "<< triangles[NbLine-NbPoint-2].V2.y
						     << " z: " << triangles[NbLine-NbPoint-2].V2.z
						     << endl;
						cout << triangles[NbLine-NbPoint-2].index
						     <<"  V3: x " << triangles[NbLine-NbPoint-2].V3.x
						     << " y "<< triangles[NbLine-NbPoint-2].V3.y
						     << " z: " << triangles[NbLine-NbPoint-2].V3.z
						     << endl;

						cout << "Triangle big   "
						     << triangles[NbLine-NbPoint-2].big.x
						     << " y "<< triangles[NbLine-NbPoint-2].big.y
						     << "z: " << triangles[NbLine-NbPoint-2].big.z
						     << endl;
						cout << "Triangle small "
						     << triangles[NbLine-NbPoint-2].small.x
						     << " y " << triangles[NbLine-NbPoint-2].small.y
						     << " z " << triangles[NbLine-NbPoint-2].small.z
						     << endl;
						//materialId is took from scene.dsc file
						cout << "MaterialId "
						     << triangles[NbLine-NbPoint-2].materialId << endl;
					}
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
		printfdeb( "can't open scene description file %s \n", sceneDescName );
		return 0;
	}

	NbLine = 0;

	while ( fgets( line, 100, fdesc )!= NULL )
	{
		if( NbLine == 0 )
		{
			printfdeb( "first line %s", line );
		}
		else
		{
			if( NbLine == 1 )
			{
				sscanf( line, "%d %d", &myScene->sizex, &myScene->sizey );
				printfdeb( "sizex: %d sizey: %d\n", myScene->sizex,
				           myScene->sizey );
			}
			else
			{
				if( NbLine == 2 )
				{
					sscanf( line, "%d %d", &myScene->NbMaterials,
					        &myScene->NbLights );
					printfdeb( "NbMaterials: %d NbLights: %d\n",
					           myScene->NbMaterials, myScene->NbLights );
				}
				else
				{
					if( NbLine > 2 && NbLine < myScene->NbMaterials + 3 )
					{
						sscanf( line, "%f %f %f %f %f %f", &v[0],
						        &v[1], &v[2], &v[3], &v[4], &v[5] );
						for( i = 0; i < 6; i++ )
						{
							fv[i] = v[i];
							//coutdeb << "fv[" << i << "] = " << fv[i] << endl;
						}

						myScene->materials[ NbLine - 3 ].red   			= fv[0];
						myScene->materials[ NbLine - 3 ].green 			= fv[1];
						myScene->materials[ NbLine - 3 ].blue  			= fv[2];
						myScene->materials[ NbLine - 3 ].reflection 	= fv[3];
						myScene->materials[ NbLine - 3 ].transmission 	= fv[4];
						myScene->materials[ NbLine - 3 ].nrefraction 	= fv[5];

						coutdeb << "M: red "
						        << myScene->materials[ NbLine - 3 ].red
						        << " green: "
						        << myScene->materials[ NbLine - 3 ].green
						        << " blue: "
						        << myScene->materials[ NbLine - 3 ].blue
						        << " reflection: "
						        << myScene->materials[ NbLine - 3 ].reflection
						        << " transmission: "
						        << myScene->materials[ NbLine - 3 ].transmission
						        << " refraction: "
						        << myScene->materials[ NbLine - 3 ].nrefraction
						        << endl;
					}
					else
					{
						if( NbLine > 2 + myScene->NbMaterials && NbLine <
						    myScene->NbMaterials + myScene->NbLights + 3 )
						{
							sscanf( line, "%f %f %f %f %f %f", &v[0], &v[1],
							        &v[2], &v[3], &v[4], &v[5] );
							for( i = 0; i < 6; i++ )
							{
								fv[i] = v[i];
								// coutdeb << "fv["<< i<< "] = " << fv[i]
								//         << endl;
							}

							myScene->lights[ NbLine - ( 3 + myScene->NbMaterials ) ].pos.x   	= fv[0];
							myScene->lights[ NbLine - ( 3 + myScene->NbMaterials ) ].pos.y 		= fv[1];
							myScene->lights[ NbLine - ( 3 + myScene->NbMaterials ) ].pos.z  	= fv[2];
							myScene->lights[ NbLine - ( 3 + myScene->NbMaterials ) ].red 		= fv[3];
							myScene->lights[ NbLine - ( 3 + myScene->NbMaterials ) ].green 		= fv[4];
							myScene->lights[ NbLine - ( 3 + myScene->NbMaterials ) ].blue 		= fv[5];

							coutdeb << "L: x "<< myScene->lights[ NbLine - ( 3 + myScene->NbMaterials ) ].pos.x << " y: " << myScene->lights[ NbLine - ( 3 + myScene->NbMaterials ) ].pos.y << " z: " << myScene->lights[ NbLine - ( 3 + myScene->NbMaterials ) ].pos.z << " red: " << myScene->lights[ NbLine - ( 3 + myScene->NbMaterials ) ].red << " green: " << myScene->lights[ NbLine - ( 3 + myScene->NbMaterials ) ].green << " blue: " << myScene->lights[ NbLine - ( 3 + myScene->NbMaterials ) ].blue << endl;
						}
						else

						{
							if( NbLine == myScene->NbMaterials + myScene->NbLights + 3 )
							{
								sscanf( line, "%d", &myScene->NbTriangles );
								printfdeb( "NbTriangles: %d \n", myScene->NbTriangles );
							}
							else
							{
								if( NbLine > (myScene->NbMaterials
								              + myScene->NbLights + 3)
								    && NbLine < (myScene->NbMaterials
								                 + myScene->NbLights
								                 + myScene->NbTriangles + 4) )
								{
									sscanf( line, "%d", &myScene->triangles[ NbLine - ( myScene->NbMaterials + myScene->NbLights + 4 ) ].materialId );
									printfdeb( "triangle[%d] materialId: %d\n", NbLine - ( myScene->NbMaterials + myScene->NbLights + 4 ), myScene->triangles[ NbLine - ( myScene->NbMaterials + myScene->NbLights + 4 ) ].materialId );
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
	printfdeb( "reading triangles\n" );
	int TrianglesExist = getTriangle ( trigFileName,
	                                   &( myScene->NbTriangles ),
	                                   myScene->triangles );
	if ( TrianglesExist == 0 )
	{
		printfdeb( "Scene file : Bad Triangle section\n" );
		return false;
	}

	return true;
}
