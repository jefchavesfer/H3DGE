#include "CoprocAdapter.hh"
#include "xparameters.h"
#include <string.h>

#define COPROC_BASEADDR XPAR_H3DGE_COPROC_0_BASEADDR
#define COPROC_WRITEREG(REG, Value)                         \
	H3DGE_COPROC_mWriteSlaveReg ## REG(COPROC_BASEADDR,     \
	                                   0,                   \
	                                   *((const Xuint32 *)(&(Value))))

#define COPROC_READREG(REG) \
	H3DGE_COPROC_mReadSlaveReg ## REG(COPROC_BASEADDR, 0)

void CoprocAdapter::setRay(const struct Ray &ray) {
	COPROC_WRITEREG(0, ray.start.x);
	COPROC_WRITEREG(1, ray.start.y);
	COPROC_WRITEREG(2, ray.start.z);
	COPROC_WRITEREG(3, ray.direction.x);
	COPROC_WRITEREG(4, ray.direction.y);
	COPROC_WRITEREG(5, ray.direction.z);
}

void CoprocAdapter::setTriangle(const Triangle &triangle) {
	COPROC_WRITEREG(6, triangle.vertices.v1.x);
	COPROC_WRITEREG(7, triangle.vertices.v1.y);
	COPROC_WRITEREG(8, triangle.vertices.v1.z);
	COPROC_WRITEREG(9, triangle.vertices.v2.x);
	COPROC_WRITEREG(10, triangle.vertices.v2.y);
	COPROC_WRITEREG(11, triangle.vertices.v2.z);
	COPROC_WRITEREG(12, triangle.vertices.v3.x);
	COPROC_WRITEREG(13, triangle.vertices.v3.y);
	COPROC_WRITEREG(14, triangle.vertices.v3.z);
	COPROC_WRITEREG(15, triangle.medium1Index);
	COPROC_WRITEREG(16, triangle.medium2Index);
}

bool CoprocAdapter::isIntersected() {
	Xuint32 coprocIsIntersected = COPROC_READREG(0);
	return coprocIsIntersected == 0x00000001;
}

bool CoprocAdapter::isReflected() {
	Xuint32 coprocIsReflected = COPROC_READREG(1);
	return coprocIsReflected == 0x00000001;
}

bool CoprocAdapter::isRefracted() {
	Xuint32 coprocIsRefracted = COPROC_READREG(1);
	return coprocIsRefracted == 0x00000001;
}

point CoprocAdapter::getIntersection() {
	point intersection;
	Xuint32 *intersectionX = (Xuint32 *)&intersection.x;
	Xuint32 *intersectionY = (Xuint32 *)&intersection.y;
	Xuint32 *intersectionZ = (Xuint32 *)&intersection.z;

	*intersectionX = COPROC_READREG(3);
	*intersectionY = COPROC_READREG(4);
	*intersectionZ = COPROC_READREG(5);

	return intersection;
}

vector CoprocAdapter::getReflectedDirection() {
	vector reflectedDirection;
	Xuint32 *reflectedDirectionX = (Xuint32 *)&reflectedDirection.x;
	Xuint32 *reflectedDirectionY = (Xuint32 *)&reflectedDirection.y;
	Xuint32 *reflectedDirectionZ = (Xuint32 *)&reflectedDirection.z;

	*reflectedDirectionX = COPROC_READREG(6);
	*reflectedDirectionY = COPROC_READREG(7);
	*reflectedDirectionZ = COPROC_READREG(8);

	return reflectedDirection;
}

vector CoprocAdapter::getRefractedDirection() {
	vector refractedDirection;
	Xuint32 *refractedDirectionX = (Xuint32 *)&refractedDirection.x;
	Xuint32 *refractedDirectionY = (Xuint32 *)&refractedDirection.y;
	Xuint32 *refractedDirectionZ = (Xuint32 *)&refractedDirection.z;

	*refractedDirectionX = COPROC_READREG(6);
	*refractedDirectionY = COPROC_READREG(7);
	*refractedDirectionZ = COPROC_READREG(8);

	return refractedDirection;
}
