#pragma once
#include <stddef.h>

struct Framebuffer {
	void* BaseAddress;
	size_t Size;
	unsigned int Width, Height, PixelsPerScanLine;
};

class GUI {
	public:
		void DrawRectangle(int x, int y, int width, int height, unsigned int color);
		void DrawCursor(int x, int y, unsigned int borderColor, unsigned int fillColor);
		void DrawLogo(int x, int y);
		void SetFramebuffer(Framebuffer* framebuffer);
		void putpixel(int x, int y, unsigned int color);
	private:
		Framebuffer* framebuffer;
};