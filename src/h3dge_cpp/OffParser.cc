#include "OffParser.hh"
#include <string>
#include <iostream>
using namespace std;

bool OffParser::parse(struct Scene &scene) {
#define READ(var) if ((stream >> (var)).fail()) return false
	static point vertices[3 * SCENE_MAX_TRIANGLES];

	string off;
	unsigned int vertexCount, faceCount, edgeCount;

	READ(off);

	// Magic keyword
	if (off != "OFF")
		return false;

	// Read vertex count, face count and edge count
	READ(vertexCount);
	READ(faceCount);
	READ(edgeCount);

	for (unsigned int i = 0; i < vertexCount; i++) {
		READ(vertices[i].x);
		READ(vertices[i].y);
		READ(vertices[i].z);
	}

	for (unsigned int i = 0; i < faceCount; i++) {
		int nEdges;

		READ(nEdges);

		// We only like triangles.
		if (nEdges != 3)
			return false;

		for (int j = 0; j < 3; j++) {
			int index;

			READ(index);
			scene.triangles[i].vertices[j] = vertices[index];
		}
	}

#undef READ(var)

	return true;
}
