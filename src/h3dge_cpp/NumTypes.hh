#ifndef _NUMTYPES_HH_
#define _NUMTYPES_HH_

#include <cfloat>
#include "FxNum.hh"

#define NUMTYPES_PRECISION 0.000001

// #define USE_FLOAT

#ifdef USE_FLOAT
typedef float color_ch;

typedef float coeff;

typedef float coord;
#else
typedef fxnum<5> color_ch;

typedef fxnum<5> coeff;

typedef fxnum<4> coord;
#endif

typedef float scalar;
#define NUMTYPES_SCALAR_MAX FLT_MAX

template<typename T1, typename T2>
inline bool isNear(const T1& n1, const T2& n2) {
	return (((n2 - NUMTYPES_PRECISION) <= n1)
	        && (n1 <= (n2 + NUMTYPES_PRECISION)));
}

template<typename T1, typename T2>
inline bool isWithin(const T2& min, const T1& n, const T2& max) {
	return (((min - NUMTYPES_PRECISION) <= n)
	        && (n <= (max + NUMTYPES_PRECISION)));
}
#endif
