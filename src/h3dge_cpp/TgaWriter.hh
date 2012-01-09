#ifndef _TGAWRITER_HH_
#define _TGAWRITER_HH_

#include <ostream>
#include <stdint.h>

class TgaWriter {
	std::ostream &stream;
public:
	explicit TgaWriter(std::ostream &_stream) : stream(_stream) {};
	bool write(uint32_t *frameBuffer, unsigned int width, unsigned int height);
};

#endif
