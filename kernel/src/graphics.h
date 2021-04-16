#pragma once
#include <stddef.h>
#include <stdint.h>
#include "math.h"
#include "bootloader.h"

struct Point {
    unsigned int X = 0;
    unsigned int Y = 0;
};

struct Cursor {
    unsigned int X = 0;
    unsigned int Y = 0;
};

class GUI {
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
		void printf(const char* str);
		unsigned int *png_parse(unsigned char *ptr, int size);
		void DrawCircle(int x0, int y0, int radius, unsigned int color, bool filled);
		void SwapBuffers();
		unsigned int GetWidth();
		unsigned int GetHeight();
		void* GetBaseAddress();
		unsigned int GetPPSL();
		size_t GetSize();
	private:
		void DrawRectangleNoCorners(int x, int y, int width, int height, unsigned int color, int radiusL, int radiusR, int radiusBL, int radiusBR);
		void putchar(char c, unsigned int xOff, unsigned int yOff, unsigned int initCursorPosX);
		Framebuffer* framebuffer;
		Point PointPosition;
		Cursor CursorPosition;
		PSF1_FONT* font;
		unsigned int color = 0xFFFFFF;
};

extern GUI* gui;

class Box {
	public:
        Box();
        void SetWidth(unsigned int width);
        void SetHeight(unsigned int height);
        void SetRadius(unsigned int rtl, unsigned int rtr, unsigned int rbl, unsigned int rbr);
        void SetRadius(unsigned int radius);
        void render();
    private:
        unsigned int x, y, width, height, rtl, rtr, rbl, rbr, color;
};