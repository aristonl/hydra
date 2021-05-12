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
#include "../misc/bootloader.hpp"

struct Point {
    unsigned int X = 0;
    unsigned int Y = 0;
};

class Graphics {
	public:
		void Initialize(Framebuffer* framebuffer, PSF1_FONT* font);
		void DrawRectangleFromTo(int x, int y, int width, int height, unsigned int color);
		void DrawCursor(int x, int y);
		void putpixel(int x, int y, unsigned int color);
		unsigned int getpixel(unsigned int x, unsigned int y);
		void putchar(char c, unsigned int xOff, unsigned int yOff, unsigned int initCursorPosX);
		void printf(const char* str);
		Point TextPosition = {0, 0};
		unsigned int color = 0xFFFFFF;
		Framebuffer* framebuffer;
	private:
		PSF1_FONT* font;
};

extern Graphics* graphics;