#pragma once
#include <stddef.h>

struct Framebuffer {
	void* BaseAddress;
	size_t Size;
	unsigned int Width, Height, PixelsPerScanLine;
};

struct PSF1_HEADER {
	unsigned char magic[2];
	unsigned char mode;
	unsigned char charsize;
};

struct PSF1_FONT {
	PSF1_HEADER* psf1_Header;
	void* glyphBuffer;
};

struct BootData {
	struct Framebuffer* framebuffer;
	struct PSF1_FONT* font;
};