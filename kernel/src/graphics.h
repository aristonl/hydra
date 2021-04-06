#pragma once
#include <stddef.h>
#include <stdint.h>
#include "math.h"
#include "bootloader.h"

struct Point {
    unsigned int X;
    unsigned int Y;
};

class GUI {
	public:
		void DrawRectangle(int x, int y, int width, int height, unsigned int color);
		void DrawRectangleFromTo(int x, int y, int width, int height, unsigned int color);
		void DrawBox(int x, int y, int width, int height, unsigned int color, int radius);
		void DrawCursor(int x, int y, int64_t style);
		void DrawLogo(int x, int y);
		void SetFramebuffer(Framebuffer* framebuffer);
		void putpixel(int x, int y, unsigned int color);
		void SetFont(PSF1_FONT* newFont);
		void SetColor(unsigned int newColor);
		void SetX(unsigned int x);
		void SetY(unsigned int y);
		void SetXY(unsigned int x, unsigned int y);
		unsigned int GetX();
		unsigned int GetY();
		void printf(const char* str);
		unsigned int *png_parse(unsigned char *ptr, int size);
		void DrawCircle(int x0, int y0, int radius, unsigned int color, bool filled);
		void SwapBuffers();
	private:
		void DrawRectangleNoCorners(int x, int y, int width, int height, unsigned int color, int radius);
		void putchar(char c, unsigned int xOff, unsigned int yOff);
		Framebuffer* framebuffer;
		Point TextCursorPosition;
		PSF1_FONT* font;
		unsigned int color = 0xFFFFFF;
};