#pragma once
#include <stddef.h>
#include <stdint.h>
#include "math.h"

struct Framebuffer {
	void* BaseAddress;
	size_t Size;
	unsigned int Width, Height, PixelsPerScanLine;
};

struct Point {
    unsigned int X;
    unsigned int Y;
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

class GUI {
	public:
		void DrawRectangle(int x, int y, int width, int height, unsigned int color);
		void DrawRectangleFromTo(int x, int y, int width, int height, unsigned int color);
		void DrawBox(int x, int y, int width, int height, unsigned int color, int radius);
		void DrawCursor(int x, int y, int64_t style);
		void DrawLogo(int x, int y);
		void SetFramebuffer(Framebuffer* framebuffer);
		void putpixel(int x, int y, unsigned int color, unsigned int* buffer);
		void SetFont(PSF1_FONT* newFont);
		void SetColor(unsigned int newColor);
		void SetX(unsigned int x);
		void SetY(unsigned int y);
		void SetXY(unsigned int x, unsigned int y);
		void printf(const char* str);
		unsigned int *png_parse(unsigned char *ptr, int size);
		void DrawCircle(int x0, int y0, int radius, unsigned int color, bool filled);
		void SwapBuffers();
	private:
		void DrawRectangleNoCorners(int x, int y, int width, int height, unsigned int color, int radius);
		void putchar(char c, unsigned int xOff, unsigned int yOff);
		unsigned int* backBuffer = (unsigned int*)framebuffer->BaseAddress+2149561088;
		unsigned int* frontBuffer = (unsigned int*)framebuffer->BaseAddress;
		Framebuffer* framebuffer;
		Point TextCursorPosition;
		PSF1_FONT* font;
		unsigned int color = 0xFFFFFF;
};