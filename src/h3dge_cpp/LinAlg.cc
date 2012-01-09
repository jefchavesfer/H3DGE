#include "LinAlg.hh"

std::ostream& operator << (std::ostream& stream, const point &p) {
	stream << "(" << p.x << ", " << p.y << ", " << p.z << ")";
	return stream;
}


std::ostream& operator << (std::ostream& stream, const vector &v) {
	stream << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	return stream;
}
