#include "TgaWriter.hh"

bool TgaWriter::write(uint32_t *frameBuffer,
                      unsigned int width, unsigned int height) {

	// Write the TGA header

	stream.put(0).put(0);
	// RGB not compressed
	stream.put(2);

	stream.put(0).put(0);
	stream.put(0).put(0);
	stream.put(0);

	// origin X
	stream.put(0).put(0);
	// origin Y
	stream.put(0).put(0);

	stream.put((unsigned char)(width & 0x00FF)).put(
		(unsigned char)((width & 0xFF00) / 256));
	stream.put((unsigned char)(height & 0x00FF)).put(
		(unsigned char)((height & 0xFF00) / 256));

	// 24 bit bitmap
	stream.put(24);
	stream.put(0x20);
	// end of the TGA header

	for (unsigned int i = 0; i < width * height; i++) {
		uint32_t currentPixel = frameBuffer[i];
		unsigned char blue = currentPixel & 0x000000ff;
		unsigned char green = (currentPixel & 0x0000ff00) >> 8;
		unsigned char red = (currentPixel & 0x00ff0000) >> 16;

		stream.put(blue).put(green).put(red);
	}

	return true;
}
