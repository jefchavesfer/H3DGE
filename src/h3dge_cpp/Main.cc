#include <iostream>
#include <fstream>
#include "Scene.hh"
#include "DscxParser.hh"
#include "OffParser.hh"
#include "RayTracer.hh"
#include "TgaWriter.hh"
using namespace std;

int main(int argc, char* argv[])
{
	static struct Scene scene;
	istream *dscxStream, *offStream;
	ifstream dscxFile, offFile;
	static uint32_t frameBuffer[SCENE_MAX_WIDTH * SCENE_MAX_HEIGHT];

	if (argc > 1) {
		dscxFile.open(argv[1]);
		dscxStream = &dscxFile;
	} else {
		dscxStream = &cin;
	}

	cout << "Parsing DSCX... ";

	DscxParser dscxParser(*dscxStream);
	if (!dscxParser.parse(scene)) {
		cerr << "Error: could not parse DSCX file." << endl;
		exit(EXIT_FAILURE);
	}

	cout << "done." << endl;

	if (argc > 2) {
		offFile.open(argv[2]);
		offStream = &offFile;
	} else {
		offStream = &cin;
	}

	cout << "Parsing OFF... ";

	OffParser offParser(*offStream);
	if (!offParser.parse(scene)) {
		cerr << "Error: could not parse OFF file." << endl;
		exit(EXIT_FAILURE);
	}

	cout << "done." << endl;

	cout << "Ray tracing scene... ";
	fflush(stdout);

	static RayTracer rayTracer(scene, SCENE_MAX_WIDTH);
	if (!rayTracer.rayTrace(frameBuffer)) {
		cerr << "Error: could not ray trace scene." << endl;
		exit(EXIT_FAILURE);
	}

	cout << "done." << endl;

	if (argc > 3) {
		ofstream tgaFile;
		tgaFile.open(argv[3]);

		cout << "Writing TGA file... ";

		TgaWriter tgaWriter(tgaFile);
		if (!tgaWriter.write(frameBuffer, scene.width, scene.height)) {
			cerr << "Error: could not write TGA file." << endl;
			exit(EXIT_FAILURE);
		}

		tgaFile.close();

		cout << "done." << endl;
	}

	if (argc > 1)
		dscxFile.close();

	if (argc > 2)
		offFile.close();

	return EXIT_SUCCESS;
}
