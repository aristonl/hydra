#include "graphics.h"

void GUI::SetFramebuffer(Framebuffer* newFramebuffer) {
    framebuffer = newFramebuffer;
}

void GUI::putpixel(int x, int y, unsigned int color) {
    y+=1;
    unsigned int* pixPtr = (unsigned int*)framebuffer->BaseAddress;
    *(unsigned int*)(pixPtr + x + (y * framebuffer->PixelsPerScanLine)) = color;
}

void GUI::DrawRectangle(int x, int y, int width, int height, unsigned int color) {
    for (int j = y; j < y+height; j++) {
        for (int i = x; i < x+width; i++) {
            putpixel(i,j,color);
        }
    }
}

void GUI::DrawCursor(int x, int y, unsigned int borderColor, unsigned int fillColor) {
    // Outline
    for (int i=0; i<=16; i++) putpixel(x,y+i,borderColor);
    putpixel(x+1, y+16, borderColor);
    putpixel(x+2, y+15, borderColor);
    putpixel(x+3, y+14, borderColor);
    putpixel(x+4, y+13, borderColor);
    putpixel(x+5, y+14, borderColor);
    putpixel(x+5, y+15, borderColor);
    putpixel(x+6, y+16, borderColor);
    putpixel(x+6, y+17, borderColor);
    putpixel(x+7, y+18, borderColor);
    putpixel(x+8, y+18, borderColor);
    putpixel(x+9, y+17, borderColor);
    putpixel(x+9, y+16, borderColor);
    putpixel(x+8, y+15, borderColor);
    putpixel(x+8, y+14, borderColor);
    putpixel(x+7, y+13, borderColor);
    putpixel(x+7, y+12, borderColor);
    putpixel(x+8, y+12, borderColor);
    putpixel(x+9, y+12, borderColor);
    putpixel(x+10, y+12, borderColor);
    putpixel(x+11, y+12, borderColor);
    putpixel(x+11, y+11, borderColor);
    putpixel(x+11, y+11, borderColor);
    putpixel(x+10, y+10, borderColor);
    putpixel(x+9, y+9, borderColor);
    putpixel(x+8, y+8, borderColor);
    putpixel(x+7, y+7, borderColor);
    putpixel(x+6, y+6, borderColor);
    putpixel(x+5, y+5, borderColor);
    putpixel(x+4, y+4, borderColor);
    putpixel(x+3, y+3, borderColor);
    putpixel(x+2, y+2, borderColor);
    putpixel(x+1, y+1, borderColor);

    // Fill
    for (int i = 0; i <= 13; i++) putpixel(x+1, y+2+i, fillColor);
    for (int i = 0; i <= 11; i++) putpixel(x+2, y+3+i, fillColor);
    for (int i = 0; i <= 9; i++) putpixel(x+3, y+4+i, fillColor);
    for (int i = 0; i <= 7; i++) putpixel(x+4, y+5+i, fillColor);
    for (int i = 0; i <= 7; i++) putpixel(x+5, y+6+i, fillColor);
    for (int i = 0; i <= 8; i++) putpixel(x+6, y+7+i, fillColor);
    for (int i = 0; i <= 3; i++) putpixel(x+7, y+8+i, fillColor);
    for (int i = 0; i <= 2; i++) putpixel(x+8, y+9+i, fillColor);
    for (int i = 0; i <= 1; i++) putpixel(x+9, y+10+i, fillColor);
    putpixel(x+10, y+11, fillColor);
    putpixel(x+7, y+14, fillColor);
    putpixel(x+7, y+15, fillColor);
    putpixel(x+7, y+16, fillColor);
    putpixel(x+7, y+17, fillColor);
    putpixel(x+8, y+16, fillColor);
    putpixel(x+8, y+17, fillColor);
}