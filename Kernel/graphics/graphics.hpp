/*
 * Copyright Null LLC
 * Please read the License!
 *  _     _           _ 
 * | |   | |         | |
 * | |__ | |_   _  _ | | ____ ____
 * |  __)| | | | |/ || |/ ___) _  |     graphics.hpp
 * | |   | | |_| ( (_| | |  ( ( | |     Controls basic GUI functions.
 * |_|   |_|\__  |\____|_|   \_||_|         Will be replaced soon!
 *         (____/
 */

#pragma once
#include <stddef.h>
#include <stdint.h>
#include "../misc/math.hpp"
#include "../misc/bootloader.hpp"

struct Point {
    unsigned int X = 0;
    unsigned int Y = 0;
};

class Graphics {
	public:
		void DrawRectangle(int x, int y, int width, int height, unsigned int color);
		void DrawRectangleFromTo(int x, int y, int width, int height, unsigned int color);
		void DrawBox(int x, int y, int width, int height, unsigned int color, int radiusL, int radiusR, int radiusBL, int radiusBR);
		void DrawCursor(int x, int y, int64_t style);
		void DrawLogo(int x, int y);
		void SetFramebuffer(Framebuffer* framebuffer);
		void putpixel(int x, int y, unsigned int color);
		unsigned int getpixel(unsigned int x, unsigned int y);
		void SetFont(PSF1_FONT* newFont);
		void SetColor(unsigned int newColor);
		void SetX(unsigned int x);
		void SetY(unsigned int y);
		void SetXY(unsigned int x, unsigned int y);
		unsigned int GetX();
		unsigned int GetY();
		void putchar(char c, unsigned int xOff, unsigned int yOff, unsigned int initCursorPosX);
		void printf(const char* str);
		void printf(char* str);
		unsigned int *png_parse(unsigned char *ptr, int size);
		void DrawCircle(int x0, int y0, int radius, unsigned int color, bool filled);
		void SwapBuffers();
		unsigned int GetWidth();
		unsigned int GetHeight();
		void* GetBaseAddress();
		unsigned int GetPPSL();
		size_t GetSize();
		unsigned int x,y;
		void test();
	private:
		void DrawRectangleNoCorners(int x, int y, int width, int height, unsigned int color, int radiusL, int radiusR, int radiusBL, int radiusBR);
		Framebuffer* framebuffer;
		Point PointPosition;
		PSF1_FONT* font;
		unsigned int color = 0xFFFFFF;
};

extern Graphics* graphics;