//operations
#ifndef __DEFTYPES_H
#define __DEFTYPES_H

#define DEBUG 0
#define printfdeb if( DEBUG ) printf
#define coutdeb if( DEBUG ) cout

#define BT 30
#define BI 4
#define MAXSTRING 100

#define MAXTRIANGLES 60000
#define MAXPOINTS 3 * MAXTRIANGLES
#define MAXLIGHTS 16
#define MAXMATERIALS 16

#define KDTREEDEPTH 16
#define MAXDEPTH 5

#define MAXNODES ( 1 << ( KDTREEDEPTH - 1 ) )

#define MAXTRIGREFS 2*MAXTRIANGLES
#define MAXRAYS (1 << 5) + 1
#define EPSILON 0.00000001 //minimal value for t

#define LIMIT_SCALE 1000
#define INSIDE 1
#define OUTSIDE 2
#define	ROOT_RAY 0
#define REFLECTED_RAY 1
#define REFRACTED_RAY 2
#define TOTAL_REFLECTED_RAY 3

#define SC_INCLUDE_FX
#include "systemc.h"
#include <iostream>

using namespace std;

//fixed precision for stocking coordinates
#define coord sc_dt::sc_fixed< 30,4, sc_dt::SC_TRN, sc_dt::SC_WRAP >

//fixed precision for stocking integer values
#define integer sc_dt::sc_fixed< 32,32, sc_dt::SC_TRN, sc_dt::SC_WRAP >

//fixed precision for stocking results of normalized vectors operations - small fixed value - sfv
#define sfv sc_dt::sc_fixed< 20, 4, sc_dt::SC_TRN, sc_dt::SC_WRAP >

//medium fixed precision. Used for stocking intensity coefficients and colors
#define mfp sc_dt::sc_fixed< 21,5, sc_dt::SC_TRN, sc_dt::SC_WRAP >

//presicion for results of divisions
#define divp sc_dt::sc_fixed< 32,16, sc_dt::SC_TRN, sc_dt::SC_WRAP >

//for calculating the SAV for dimensioning it is supposed a minimum coordinate division of 0.001 and a maximum of 1
#define surfAreaTot sc_dt::sc_ufixed< 28, 20, sc_dt::SC_TRN, sc_dt::SC_WRAP >

//for calculating the SAV for dimensioning it is supposed a minimum coordinate division of 0.001 and a maximum of 1
#define surfArea sc_dt::sc_ufixed< 15, 4, sc_dt::SC_TRN, sc_dt::SC_WRAP >

//dimension for cost variables
//16 bits for integer - for representing the maximun of 60k inside a unique leaf
//6 fraction bits - for a precision of 0.01
#define cost sc_dt::sc_ufixed< 22, 16, sc_dt::SC_TRN, sc_dt::SC_WRAP >

#define ifixed sc_dt::sc_fix

typedef struct _point {
	float x, y, z;
} point;

typedef struct _point_fixed
{
	 coord x, y, z;
} point_fixed;

typedef struct _vect {
	float x, y, z;

    struct _vect& operator += (const struct _vect &v2){
	    this->x += v2.x;
        this->y += v2.y;
        this->z += v2.z;
	    return *this;
    }
} vect;

typedef struct _vect_fixed {
	coord x, y, z;

    struct _vect_fixed& operator += (const struct _vect_fixed &v2){
	    this->x += v2.x;
        this->y += v2.y;
        this->z += v2.z;
	    return *this;
    }
} vect_fixed;

inline point operator + (const point&p, const vect &v){
	point p2={p.x + v.x, p.y + v.y, p.z + v.z };
	return p2;
}

inline point_fixed operator + (const point_fixed &p, const vect_fixed &v){
	point_fixed p2={p.x + v.x, p.y + v.y, p.z + v.z };
	return p2;
}

inline point operator - (const point&p, const vect &v){
	point p2={p.x - v.x, p.y - v.y, p.z - v.z };
	return p2;
}

inline point_fixed operator - (const point_fixed &p, const vect_fixed &v){
	point_fixed p2={p.x - v.x, p.y - v.y, p.z - v.z };
	return p2;
}

inline vect operator + (const vect&v1, const vect &v2){
	vect v={v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
	return v;
}

inline vect_fixed operator + (const vect_fixed&v1, const vect_fixed &v2){
	vect_fixed v={v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
	return v;
}

inline vect operator - (const point&p1, const point &p2){
	vect v={p1.x - p2.x, p1.y - p2.y, p1.z - p2.z };
	return v;
}

inline vect_fixed operator - (const point_fixed&p1, const point_fixed &p2){
	vect_fixed v={p1.x - p2.x, p1.y - p2.y, p1.z - p2.z };
	return v;
}

inline vect operator * (float c, const vect &v)
{
	vect v2={v.x *c, v.y * c, v.z * c };
	return v2;
}

inline vect_fixed operator * (coord c, const vect_fixed &v)
{
	vect_fixed v2={v.x *c, v.y *c, v.z * c };
	return v2;
}

inline vect operator - (const vect&v1, const vect &v2){
	vect v={v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
	return v;
}

inline vect_fixed operator - (const vect_fixed&v1, const vect_fixed &v2){
	vect_fixed v={v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
	return v;
}

inline float operator * (const vect&v1, const vect &v2 ) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline coord operator * (const vect_fixed&v1, const vect_fixed &v2 ) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}



typedef struct _color {
    enum OFFSET
    {
        OFFSET_RED = 0,
        OFFSET_GREEN = 1,
        OFFSET_BLUE = 2,
        OFFSET_MAX  = 3
    };
    float red, green, blue;

    inline struct _color & operator += (const struct _color &c2 ) {
	    this->red +=  c2.red;
        this->green += c2.green;
        this->blue += c2.blue;
	    return *this;
    }


    inline float & getChannel(OFFSET offset )
    {
        return reinterpret_cast<float*>(this)[offset];
    }

    inline float getChannel(OFFSET offset ) const
    {
        return reinterpret_cast<const float*>(this)[offset];
    }
} color;


typedef struct _color_fixed {
    enum OFFSET
    {
        OFFSET_RED = 0,
        OFFSET_GREEN = 1,
        OFFSET_BLUE = 2,
        OFFSET_MAX  = 3
    };
    mfp red, green, blue;

    inline struct _color_fixed & operator += (const struct _color_fixed &c2 ) {
	    this->red +=  c2.red;
        this->green += c2.green;
        this->blue += c2.blue;
	    return *this;
    }

} color_fixed;

typedef struct _ray {
	point_fixed start;
	vect_fixed dir;
} ray;

inline color operator * (const color&c1, const color &c2 ) {
	color c = {c1.red * c2.red, c1.green * c2.green, c1.blue * c2.blue};
	return c;
}

inline color_fixed operator * (const color_fixed&c1, const color_fixed &c2 ) {
	color_fixed c = {c1.red * c2.red, c1.green * c2.green, c1.blue * c2.blue};
	return c;
}

inline color operator + (const color&c1, const color &c2 ) {
	color c = {c1.red + c2.red, c1.green + c2.green, c1.blue + c2.blue};
	return c;
}

inline color_fixed operator + (const color_fixed&c1, const color_fixed &c2 ) {
	color_fixed c = {c1.red + c2.red, c1.green + c2.green, c1.blue + c2.blue};
	return c;
}

inline color operator * (float coef, const color &c ) {
	color c2 = {c.red * coef, c.green * coef, c.blue * coef};
	return c2;
}

inline color_fixed operator * (mfp coef, const color_fixed &c ) {
	color_fixed c2 = {c.red * coef, c.green * coef, c.blue * coef};
	return c2;
}

typedef struct _material {
    mfp reflection;
	mfp red, green, blue;
	mfp transmission;
	mfp nrefraction;
} material;

typedef struct _light {
	point_fixed pos;
	mfp red, green, blue;
} light;

/*typedef struct _ray_fixed {
	point_fixed start;
	vect_fixed dir;
} ray_fixed;*/

typedef struct _triangle
{
	//for debugging. Best way to verify if the triangles are different
	int index;

	//for bounding the triangle

	//stocks the biggest coordinates of each axis
	point_fixed big;

	//stocks the smallest coordinates of each axis
	point_fixed small;

	// vertices coordinates
	point_fixed V1, V2, V3;

	//material identification
	int materialId;
} triangle;

//this structure stocks the references for the triangles
//inside a leaf kdTreeNode
typedef struct _TrigRefList
{
	triangle *ref;
	struct _TrigRefList* next;
} TrigRefList;

//node for spatial indexing
typedef struct _kdTreeNode
{
	struct _kdTreeNode * left; //pointers to left and right tree nodes
	struct _kdTreeNode * right;
	struct _kdTreeNode * next; //only used for free node stocking

	TrigRefList * trig; //only used if the node is on the end of a branch
	int pnum; //pointer to the set of 'pnum' triangles contained in the leaf.

	coord x0, x1;
	coord y0, y1;
	coord z0, z1;

	int spx; //used only in rendering and traversal, what axis it was split.
} kdTreeNode;

typedef struct _node
{
	struct _node * parent;
	struct _node * left;
	struct _node * right;
	struct _node * next;
	int type;
	ray v;
	kdTreeNode* currentPacket;
	int level;
	int visited;
	int state;
	triangle* lastIntercepted;
	coord t;
} node;


//struct that stocks possible intersection point
//I     - possible intersection point
//valid - it means if this point is inside the triangle packet or not
typedef struct _pairInt
{
	point_fixed I;
	bool valid;
} pairInt;

//struct that stocks possible splitting point
//split pos - possible splitting point
//n1count   - number of triangles in the LEFT site
//n2count   - number of triangles in the RIGHT site
typedef struct _SplitList
{
	coord splitpos;
	unsigned int n1count, n2count;
	struct _SplitList* next;
} SplitList;

kdTreeNode* initFreeNodeList( kdTreeNode nodes[MAXNODES] );
node* initFreeNodeList( node rayNodes[MAXRAYS] );
TrigRefList* initFreeNodeList( TrigRefList nodes[MAXTRIGREFS] );

void deliverNode( kdTreeNode* node, kdTreeNode** freeNodes );
void deliverNode( TrigRefList* node, TrigRefList** freeNodes );
void deliverNode( node* rayNode, node** freeRayNodes );

kdTreeNode* requestNode( kdTreeNode** freeNodes );
TrigRefList* requestNode( TrigRefList** freeNodes );
node* requestNode( node** freeRayNodes );

int compLess( coord x, coord y );
kdTreeNode* newNode( kdTreeNode** freeNodes, const coord X0, const coord Y0, const coord Z0, const coord X1, const coord Y1, const coord Z1 );
void initNode( node* curNode, node* parent, kdTreeNode* packet, ray r, triangle* trigIntersected, int h, int type, int state);

coord square_root( coord entree );
#endif //__DEFTYPES_H
