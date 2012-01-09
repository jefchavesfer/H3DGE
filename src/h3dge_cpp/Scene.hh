#ifndef _SCENE_HH_
#define _SCENE_HH_

#include "NumTypes.hh"
#include "LinAlg.hh"
#include <ostream>

#define SCENE_MAX_WIDTH     640
#define SCENE_MAX_HEIGHT    480
#define SCENE_MAX_MATERIALS 16
#define SCENE_MAX_MEDIA     16
#define SCENE_MAX_LIGHTS    16
#define SCENE_MAX_TRIANGLES 70000

inline void saturate(color_ch &channel) {
	channel = (channel < 0) ? 0 : channel > 1.0 ? 1.0 : channel;
}

class rgb {
public:
	color_ch red;
	color_ch green;
	color_ch blue;

    inline rgb& operator += (const rgb &c2 ) {
	    red   +=  c2.red;
	    green += c2.green;
	    blue  += c2.blue;

	    saturate(red);
	    saturate(green);
	    saturate(blue);

	    return *this;
    }
};

inline rgb operator * (const rgb &c1, const rgb &c2 ) {
	rgb c = {c1.red * c2.red, c1.green * c2.green, c1.blue * c2.blue};
	saturate(c.red);
	saturate(c.green);
	saturate(c.blue);
	return c;
}

inline rgb operator * (scalar coef, const rgb &c ) {
	rgb c2 = {c.red * coef, c.green * coef, c.blue * coef};
	saturate(c2.red);
	saturate(c2.green);
	saturate(c2.blue);
	return c2;
}

inline rgb operator * (const rgb &c, scalar coef) {
	return operator * (coef, c);
}

inline rgb operator + (const rgb &c1, const rgb &c2 ) {
	rgb c = {c1.red + c2.red, c1.green + c2.green, c1.blue + c2.blue};
	saturate(c.red);
	saturate(c.green);
	saturate(c.blue);
	return c;
}

std::ostream& operator << (std::ostream &stream, const rgb &c);

struct Material {
	rgb color;
	coeff reflection;
	coeff transmission;
};

struct Light {
	point position;
	rgb color;
};

struct Medium {
	coeff nRefraction;
};

struct Triangle {
	struct {
		point v1, v2, v3;

		point& operator [] (int i) {
			assert(i >= 0);
			assert(i < 3);

			switch(i) {
			case 0:
				return v1;
			case 1:
				return v2;
			case 2:
				return v3;
			default:
				assert(0);
				exit(EXIT_FAILURE);
			}
		}
	} vertices;
	uint32_t materialIndex;
	uint32_t medium1Index;
	uint32_t medium2Index;

	vector normal() const {
		vector V1 = this->vertices.v2 - this->vertices.v1;
		vector V2 = this->vertices.v3 - this->vertices.v1;
		vector normal = V1 ^ V2;
		normal *= 1 / normal.norm();

		return normal;
	}
};

bool operator == (const struct Triangle &t1, const struct Triangle &t2);

struct Scene {
	uint32_t width;
	uint32_t height;
	uint32_t nbMaterials;
	uint32_t nbMedia;
	uint32_t nbLights;
	uint32_t nbTriangles;
	struct Material materials[SCENE_MAX_MATERIALS];
	struct Medium   media[SCENE_MAX_MEDIA];
	struct Light    lights[SCENE_MAX_LIGHTS];
	struct Triangle triangles[SCENE_MAX_TRIANGLES];
};

#endif
