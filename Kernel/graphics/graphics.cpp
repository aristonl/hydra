/*
 * Copyright Null LLC
 * Please read the License!
 *  _     _           _ 
 * | |   | |         | |
 * | |__ | |_   _  _ | | ____ ____
 * |  __)| | | | |/ || |/ ___) _  |     graphics.cpp
 * | |   | | |_| ( (_| | |  ( ( | |     Controls basic GUI functions.
 * |_|   |_|\__  |\____|_|   \_||_|         Will be replaced soon!
 *         (____/
 */

#include "graphics.hpp"
#include "../misc/string.hpp"

Graphics* graphics;

void Graphics::Initialize(Framebuffer* framebuffer, PSF1_FONT* font) {
    this->framebuffer = framebuffer;
    this->font = font;
}

void Graphics::putpixel(int x, int y, unsigned int color) {
    if (getpixel(x, y) != color) {
        if ((x >= 0 && x < this->framebuffer->Width && y < this->framebuffer->Height && y >= 0)) {
            unsigned int* pixPtr = (unsigned int*) this->framebuffer->BaseAddress;
            *(unsigned int*)(pixPtr + x + (y * this->framebuffer->PixelsPerScanLine)) = color;
        }
    }
}

unsigned int Graphics::getpixel(unsigned int x, unsigned int y) {
    unsigned int* pixPtr = (unsigned int*) this->framebuffer->BaseAddress;
    return *(unsigned int*)(pixPtr + x + (y * this->framebuffer->PixelsPerScanLine));
}

void Graphics::DrawRectangleFromTo(int x, int y, int toX, int toY, unsigned int color) {
    for (int j = y; j < toY; j++) {
        for (int i = x; i < toX; i++) {
            putpixel(i,j,color);
        }
    }
}

void Graphics::DrawCursor(int x, int y) {
    // Outline
    for (int i=0; i<=16; i++) putpixel(x,y+i,0x000000);
    putpixel(x+1, y+16, 0x000000);
    putpixel(x+2, y+15, 0x000000);
    putpixel(x+3, y+14, 0x000000);
    putpixel(x+4, y+13, 0x000000);
    putpixel(x+5, y+14, 0x000000);
    putpixel(x+5, y+15, 0x000000);
    putpixel(x+6, y+16, 0x000000);
    putpixel(x+6, y+17, 0x000000);
    putpixel(x+7, y+18, 0x000000);
    putpixel(x+8, y+18, 0x000000);
    putpixel(x+9, y+17, 0x000000);
    putpixel(x+9, y+16, 0x000000);
    putpixel(x+8, y+15, 0x000000);
    putpixel(x+8, y+14, 0x000000);
    putpixel(x+7, y+13, 0x000000);
    putpixel(x+7, y+12, 0x000000);
    putpixel(x+8, y+12, 0x000000);
    putpixel(x+9, y+12, 0x000000);
    putpixel(x+10, y+12, 0x000000);
    putpixel(x+11, y+12, 0x000000);
    putpixel(x+11, y+11, 0x000000);
    putpixel(x+11, y+11, 0x000000);
    putpixel(x+10, y+10, 0x000000);
    putpixel(x+9, y+9, 0x000000);
    putpixel(x+8, y+8, 0x000000);
    putpixel(x+7, y+7, 0x000000);
    putpixel(x+6, y+6, 0x000000);
    putpixel(x+5, y+5, 0x000000);
    putpixel(x+4, y+4, 0x000000);
    putpixel(x+3, y+3, 0x000000);
    putpixel(x+2, y+2, 0x000000);
    putpixel(x+1, y+1, 0x000000);

    // Fill
    for (int i = 0; i <= 13; i++) putpixel(x+1, y+2+i, 0xFFFFFF);
    for (int i = 0; i <= 11; i++) putpixel(x+2, y+3+i, 0xFFFFFF);
    for (int i = 0; i <= 9; i++) putpixel(x+3, y+4+i, 0xFFFFFF);
    for (int i = 0; i <= 7; i++) putpixel(x+4, y+5+i, 0xFFFFFF);
    for (int i = 0; i <= 7; i++) putpixel(x+5, y+6+i, 0xFFFFFF);
    for (int i = 0; i <= 8; i++) putpixel(x+6, y+7+i, 0xFFFFFF);
    for (int i = 0; i <= 3; i++) putpixel(x+7, y+8+i, 0xFFFFFF);
    for (int i = 0; i <= 2; i++) putpixel(x+8, y+9+i, 0xFFFFFF);
    for (int i = 0; i <= 1; i++) putpixel(x+9, y+10+i, 0xFFFFFF);
    putpixel(x+10, y+11, 0xFFFFFF);
    putpixel(x+7, y+14, 0xFFFFFF);
    putpixel(x+7, y+15, 0xFFFFFF);
    putpixel(x+7, y+16, 0xFFFFFF);
    putpixel(x+7, y+17, 0xFFFFFF);
    putpixel(x+8, y+16, 0xFFFFFF);
    putpixel(x+8, y+17, 0xFFFFFF);
}

void Graphics::putchar(char c, unsigned int xOff, unsigned int yOff, unsigned int initCursorPosX) {
    if (c == '\n') this->TextPosition = {initCursorPosX, this->TextPosition.Y+16};
    else {
        char* fontPtr = (char*) this->font->glyphBuffer + (c * this->font->psf1_Header->charsize);
        for (unsigned long y = yOff; y < yOff + 16; y++) {
            for (unsigned long x = xOff; x < xOff+8; x++) {
                if ((*fontPtr & (0b10000000 >> (x - xOff))) > 0) {
                    putpixel(x, y, color);
                }
            }
            fontPtr++;
        }
    }
    
}

void Graphics::printf(const char* str) {
    char* chr = (char*) str;
    unsigned int initCursorPosX = this->TextPosition.Y-8;
    while (*chr != 0) {
        putchar(*chr, this->TextPosition.X, this->TextPosition.Y, initCursorPosX);
        this->TextPosition.X+=8;
        if (this->TextPosition.X+8 > framebuffer->Width) {
            this->TextPosition.X = 0;
            this->TextPosition.Y += 16;
        }
        chr++;
    }
}