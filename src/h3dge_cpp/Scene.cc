#include "Scene.hh"

std::ostream& operator << (std::ostream &stream, const rgb &c) {
	stream << "(R: " << c.red
	       << "; G: " << c.green
	       << "; B: " << c.blue << ")";
	return stream;
}

bool operator == (const struct Triangle &t1, const struct Triangle &t2) {
#define T1_EQUAL(V1, V2, V3) ((t1.vertices.v1 == t2.vertices.V1)	\
                              && (t1.vertices.v2 == t2.vertices.V2)	\
                              && (t1.vertices.v3 == t2.vertices.V3))
	return (T1_EQUAL(v1, v2, v3) || T1_EQUAL(v1, v3, v2)
	        || T1_EQUAL(v2, v1, v3) || T1_EQUAL(v2, v3, v1)
	        || T1_EQUAL(v3, v1, v2) || T1_EQUAL(v3, v2, v1));
#undef T1_EQUAL
}
