#include "DscxParser.hh"
#include <string>
using namespace std;

bool DscxParser::parse(struct Scene &scene) {
#define READ(var) if ((stream >> (var)).fail()) return false

	string dscx;

	READ(dscx);

	// Magic keyword
	if (dscx != "DSCX")
		return false;

	// Read scene width and height
	READ(scene.width);
	if (scene.width > SCENE_MAX_WIDTH)
		return false;

	READ(scene.height);
	if (scene.height > SCENE_MAX_HEIGHT)
		return false;

	// Read number of materials and light sources
	READ(scene.nbMaterials);
	READ(scene.nbMedia);
	READ(scene.nbLights);

	for (unsigned int i = 0; i < scene.nbMaterials; i++) {
		READ(scene.materials[i].color.red);
		READ(scene.materials[i].color.green);
		READ(scene.materials[i].color.blue);
		READ(scene.materials[i].reflection);
		READ(scene.materials[i].transmission);
	}

	for (unsigned int i = 0; i < scene.nbMedia; i++) {
		READ(scene.media[i].nRefraction);
	}

	for (unsigned int i = 0; i < scene.nbLights; i++) {
		READ(scene.lights[i].position.x);
		READ(scene.lights[i].position.y);
		READ(scene.lights[i].position.z);
		READ(scene.lights[i].color.red);
		READ(scene.lights[i].color.green);
		READ(scene.lights[i].color.blue);
	}

	READ(scene.nbTriangles);

	for (unsigned i = 0; i < scene.nbTriangles; i++) {
		READ(scene.triangles[i].materialIndex);
		READ(scene.triangles[i].medium1Index);
		READ(scene.triangles[i].medium2Index);
	}

#undef READ(var)

	return true;
}
