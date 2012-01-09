#include <iostream>
#include <fstream>
#include <sleep.h>
#include "Scene.hh"
#include "DscxParser.hh"
#include "OffParser.hh"
#include "RayTracer.hh"
#include "TgaWriter.hh"
#include "platform.h"
#include "xtft.h"
#include "xtft_hw.h"
#include "xparameters.h"
#include "h3dge_coproc.h"
using namespace std;

int main()
{
	static struct Scene scene;
	istream *dscxStream, *offStream;
	ifstream dscxFile, offFile;
	uint32_t *frameBuffer;
	int Status;
	XTft_Config *TftConfigPtr;
	XTft TftInstance;
	uint32_t h3dge_coproc_baseaddr = XPAR_H3DGE_COPROC_0_BASEADDR;
	unsigned int result;

	init_platform();

#define FIXED(x)((Xuint32)((x) * (1 << 28)))
#define WRITEREG(N, Value) H3DGE_COPROC_mWriteSlaveReg ## N(h3dge_coproc_baseaddr, 0, (Value))
	cout << "h3dge_coproc sum test" << endl;
	H3DGE_COPROC_mWriteSlaveReg18(h3dge_coproc_baseaddr, 0, 22);
	cout << "wrote 22 to reg18" << endl;
	H3DGE_COPROC_mWriteSlaveReg19(h3dge_coproc_baseaddr, 0, 20);
	cout << "wrote 20 to reg19" << endl;
	result = H3DGE_COPROC_mReadSlaveReg19(h3dge_coproc_baseaddr, 0);
	cout << "the answer to life, the universe and everything is "
	     << result << endl;
	cout << "reset coprocessor" << endl;
	WRITEREG(0, 0x00000002);
	WRITEREG(1, 0x00000000);
	WRITEREG(2, 0x08000000);
	WRITEREG(3, 0x08000000);
	WRITEREG(4, 0x093cd3a0);
	WRITEREG(5, 0x093cd3a0);
	WRITEREG(6, 0xf6c32c60);
	WRITEREG(7, 0x00000000);
	WRITEREG(8, 0x00000000);
	WRITEREG(9, 0x00000000);
	WRITEREG(10, 0x20000000);
	WRITEREG(11, 0x00000000);
	WRITEREG(12, 0x00000000);
	WRITEREG(13, 0x00000000);
	WRITEREG(14, 0x20000000);
	WRITEREG(15, 0x00000000);
	cout << "starting coprocessor" << endl;
	WRITEREG(0, 1);

	int i = 0;
    do {
		result = H3DGE_COPROC_mReadSlaveReg0(h3dge_coproc_baseaddr, 0);
		i++;
	} while (!(result & 0x1));

	result = H3DGE_COPROC_mReadSlaveReg1(h3dge_coproc_baseaddr, 0);
	printf("result = %x\n", result);
	cout << i << endl;

	/*
	 * Get address of the XTft_Config structure for the given device id.
	 */

	TftConfigPtr = XTft_LookupConfig(XPAR_XPS_TFT_0_DEVICE_ID);
	if (TftConfigPtr == (XTft_Config *)NULL) {
		return XST_FAILURE;
	}

	/*
	 * Initialize all the TftInstance members and fills the screen with
	 * default background color.
	 */
	Status = XTft_CfgInitialize(&TftInstance, TftConfigPtr,
				 	TftConfigPtr->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	XTft_SetColor(&TftInstance, 0, 0);
	XTft_ClearScreen(&TftInstance);
	usleep(2000000);
	XTft_SetColor(&TftInstance, 0x00FFFFFF, 0x0);
	XTft_Write(&TftInstance, 'h');
	XTft_Write(&TftInstance, '3');
	XTft_Write(&TftInstance, 'd');
	XTft_Write(&TftInstance, 'g');
	XTft_Write(&TftInstance, 'e');
	usleep(3000000);

	dscxStream = &cin;

	while(1) {
		cout << "Parsing DSCX... ";

		DscxParser dscxParser(*dscxStream);
		if (!dscxParser.parse(scene)) {
			cerr << "Error: could not parse DSCX file." << endl;
			exit(EXIT_FAILURE);
		}

		cout << "done." << endl;

		offStream = &cin;

		cout << "Parsing OFF... ";

		OffParser offParser(*offStream);
		if (!offParser.parse(scene)) {
			cerr << "Error: could not parse OFF file." << endl;
			exit(EXIT_FAILURE);
		}

		cout << "done." << endl;

		cout << "Ray tracing scene... ";
		fflush(stdout);

		static RayTracer rayTracer(scene, XTFT_DISPLAY_BUFFER_WIDTH);
		frameBuffer = (uint32_t *)TftConfigPtr->VideoMemBaseAddr;
		if (!rayTracer.rayTrace(frameBuffer)) {
			cerr << "Error: could not ray trace scene." << endl;
			exit(EXIT_FAILURE);
		}

		cout << "done." << endl;

		cout << "Press <enter> to render another scene." << endl;

		while (cin.get() != '\r');

		XTft_SetColor(&TftInstance, 0, 0);
		XTft_ClearScreen(&TftInstance);
		usleep(2000000);
		XTft_SetColor(&TftInstance, 0x00FFFFFF, 0x0);
		XTft_Write(&TftInstance, 'h');
		XTft_Write(&TftInstance, '3');
		XTft_Write(&TftInstance, 'd');
		XTft_Write(&TftInstance, 'g');
		XTft_Write(&TftInstance, 'e');
	}
	cleanup_platform();

	return EXIT_SUCCESS;
}
