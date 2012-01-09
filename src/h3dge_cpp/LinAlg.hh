#ifndef _LINALG_HH_
#define _LINALG_HH_

#include "NumTypes.hh"
#include <ostream>
#include <cassert>
#include <cmath>

#define LINALG_AXIS_X 0
#define LINALG_AXIS_Y 1
#define LINALG_AXIS_Z 2

class point;
class vector;

class point {
public:
	coord x, y, z;

	coord& operator [] (int i) {
		assert((0 <= i) && (i < 3));

		switch (i) {
		case LINALG_AXIS_X:
			return x;
		case LINALG_AXIS_Y:
			return y;
		case LINALG_AXIS_Z:
			return z;
		default:
			assert(0);
			exit(EXIT_FAILURE);
		}
	}
};

inline bool operator == (const point &p1, const point &p2) {
	return (p1.x == p2.x) && (p1.y == p2.y) && (p1.z == p2.z);
}

inline bool isNear(const point &p1, const point &p2) {
	return isNear(p1.x, p2.x) && isNear(p1.y, p2.y) && isNear(p1.z, p2.z);
}

class vector {
public:
	coord x, y, z;

	coord& operator [] (int i) {
		assert((0 <= i) && (i < 3));

		switch (i) {
		case LINALG_AXIS_X:
			return x;
		case LINALG_AXIS_Y:
			return y;
		case LINALG_AXIS_Z:
			return z;
		default:
			assert(0);
			exit(EXIT_FAILURE);
		}
	}

	vector& operator += (const vector &v) {
		this->x += v.x;
		this->y += v.y;
		this->z += v.z;
		return *this;
	}

	vector& operator -= (const vector &v) {
		this->x -= v.x;
		this->y -= v.y;
		this->z -= v.z;
		return *this;
	}

	vector& operator *= (scalar s) {
		this->x *= s;
		this->y *= s;
		this->z *= s;
		return *this;
	}

	vector& operator ^= (const vector &v) {
		vector me = *this;
		this->x = me.y * v.z - me.z * v.y;
		this->y = me.z * v.x - me.x * v.z;
		this->z = me.x * v.y - me.y * v.x;
		return *this;
	}

	scalar norm() {
		return sqrt(this->x * this->x
		            + this->y * this->y
		            + this->z * this->z);
	}
};

inline vector operator - (const point &p1, const point &p2) {
	vector result = {p1.x - p2.x, p1.y - p2.y, p1.z - p2.z};
	return result;
}

inline point& operator += (point &p, const vector &v) {
	p.x += v.x;
	p.y += v.y;
	p.z += v.z;
	return p;
}

inline point operator + (const point &p, const vector &v) {
	point result = p;
	result += v;
	return result;
}

std::ostream& operator << (std::ostream& stream, const point &p);

inline vector operator + (const vector &v1, const vector &v2) {
	vector result = v1;
	result += v2;
	return result;
}

inline vector operator - (const vector &v1, const vector &v2) {
	vector result = v1;
	result -= v2;
	return result;
}

inline vector operator - (const vector &v) {
	vector result = (vector){0, 0, 0} - v;
	return result;
}

inline vector operator * (scalar s, const vector &v) {
	vector result = {s * v.x, s * v.y, s * v.z};
	return result;
}

inline vector operator * (const vector &v, scalar s) {
	return operator*(s,v);
}

inline scalar operator * (const vector &v1, const vector &v2) {
	return (v1.x * v2.x
	        + v1.y * v2.y
	        + v1.z * v2.z);
}

inline vector operator ^ (const vector &v1, const vector &v2) {
	vector result = v1;
	result ^= v2;
	return result;
}

std::ostream& operator << (std::ostream& stream, const vector &v);

#endif
