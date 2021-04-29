#include "graphics.h"
#include "string.h"

Graphics* graphics;

void Graphics::SetFont(PSF1_FONT* newFont) {
    font = newFont;
}

void Graphics::SetColor(unsigned int newColor) {
    color = newColor;
}

void Graphics::SetFramebuffer(Framebuffer* newFramebuffer) {
    framebuffer = newFramebuffer;
    x = framebuffer->Width/2;
    y = framebuffer->Height/2;
}

void Graphics::putpixel(int x, int y, unsigned int color) {
    unsigned int* pixPtr = (unsigned int*) framebuffer->BaseAddress;
    *(unsigned int*)(pixPtr + x + (y * framebuffer->PixelsPerScanLine)) = color;
}

unsigned int Graphics::getpixel(unsigned int x, unsigned int y) {
    unsigned int* pixPtr = (unsigned int*) framebuffer->BaseAddress;
    return *(unsigned int*)(pixPtr + x + (y * framebuffer->PixelsPerScanLine));
}

void Graphics::DrawRectangle(int x, int y, int width, int height, unsigned int color) {
    for (int j = y; j < y+height; j++) {
        for (int i = x; i < x+width; i++) {
            putpixel(i,j,color);
        }
    }
}

void Graphics::DrawRectangleFromTo(int x, int y, int toX, int toY, unsigned int color) {
    for (int j = y; j < toY; j++) {
        for (int i = x; i < toX; i++) {
            putpixel(i,j,color);
        }
    }
}

void Graphics::DrawCursor(int x, int y, int64_t style) {
    if (style == 0) {
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
    } else {
        if (font == NULL) return;
        SetXY(10, 10);
        printf("The style: \"");
        printf(to_string((uint64_t) style));
        printf("\", was not found!");
    }
    x = x;
    y = y;
}

void Graphics::putchar(char c, unsigned int xOff, unsigned int yOff, unsigned int initCursorPosX) {
    if (c == '\n') {
        SetXY(initCursorPosX, GetY() + 16);
    } else {
        unsigned int* pixPtr = (unsigned int*) framebuffer->BaseAddress;
        char* fontPtr = (char*) font->glyphBuffer + (c * font->psf1_Header->charsize);
        for (unsigned long y = yOff; y < yOff + 16; y++) {
            for (unsigned long x = xOff; x < xOff+8; x++) {
                if ((*fontPtr & (0b10000000 >> (x - xOff))) > 0) {
                    *(unsigned int*)(pixPtr + x + (y * framebuffer->PixelsPerScanLine)) = color;
                }
            }
            fontPtr++;
        }
    }
    
}

void Graphics::printf(const char* str) {
    char* chr = (char*) str;
    unsigned int initCursorPosX = GetY()-8;
    while (*chr != 0) {
        putchar(*chr, PointPosition.X, PointPosition.Y, initCursorPosX);
        PointPosition.X+=8;
        if (PointPosition.X+8 > framebuffer->Width) {
            PointPosition.X = 0;
            PointPosition.Y += 16;
        }
        chr++;
    }
}

void Graphics::printf(char* str) {
    char* chr = (char*) str;
    unsigned int initCursorPosX = GetY()-8;
    while (*chr != 0) {
        putchar(*chr, PointPosition.X, PointPosition.Y, initCursorPosX);
        PointPosition.X+=8;
        if (PointPosition.X+8 > framebuffer->Width) {
            PointPosition.X = 0;
            PointPosition.Y += 16;
        }
        chr++;
    }
}

void Graphics::SetX(unsigned int x) {
    PointPosition.X = x;
}

void Graphics::SetY(unsigned int y) {
    PointPosition.Y = y;
}

unsigned int Graphics::GetX() {
    return PointPosition.X;
}

unsigned int Graphics::GetY() {
    return PointPosition.Y;
}

void Graphics::SetXY(unsigned int x, unsigned int y) {
    PointPosition.X = x;
    PointPosition.Y = y;
}

void Graphics::DrawCircle(int x0, int y0, int radius, unsigned int color, bool filled) {
    if (filled) {
        int x = radius;
        int y = 0;
        int xChange = 1 - (radius << 1);
        int yChange = 0;
        int radiusError = 0;

        while (x >= y) {
            for (int i = x0 - x; i <= x0 + x; i++) {
                putpixel(i, y0 + y, color);
                putpixel(i, y0 - y, color);
            }
            for (int i = x0 - y; i <= x0 + y; i++) {
                putpixel(i, y0 + x, color);
                putpixel(i, y0 - x, color);
            }
            y++;
            radiusError += yChange;
            yChange += 2;
            if (((radiusError << 1) + xChange) > 0) {
                x--;
                radiusError += xChange;
                xChange += 2;
            }
        }
    } else {
        int x = 0, y = radius;
        int d = 3 - 2 * radius;
        putpixel(x0+x, y0+y, color);
        putpixel(x0-x, y0+y, color);
        putpixel(x0+x, y0-y, color);
        putpixel(x0-x, y0-y, color);
        putpixel(x0+y, y0+x, color);
        putpixel(x0-y, y0+x, color);
        putpixel(x0+y, y0-x, color);
        putpixel(x0-y, y0-x, color);
        while (y >= x) {
            x++;
            if (d > 0) {
                y--;
                d = d + 4 * (x - y) + 10;
            } else d = d + 4 * x + 6;
            putpixel(x0+x, y0+y, color);
            putpixel(x0-x, y0+y, color);
            putpixel(x0+x, y0-y, color);
            putpixel(x0-x, y0-y, color);
            putpixel(x0+y, y0+x, color);
            putpixel(x0-y, y0+x, color);
            putpixel(x0+y, y0-x, color);
            putpixel(x0-y, y0-x, color);
        }
    }
}

void Graphics::DrawRectangleNoCorners(int x, int y, int width, int height, unsigned int color, int radiusL, int radiusR, int radiusBL, int radiusBR) {
    for (int j = y; j < y+height; j++) {
        for (int i = x; i < x+width; i++) {
            if (i < x+radiusL && j < y+radiusL) {}
            else if (i > x+width-radiusR && j < y+radiusR) {}
            else if (i < x+radiusBL && j > y+height-radiusBL) {}
            else if (i > x+width-radiusBR && j > y+height-radiusBR) {}
            else putpixel(i,j,color);
        }
    }
}

void Graphics::DrawBox(int x, int y, int width, int height, unsigned int color, int radiusL, int radiusR, int radiusBL, int radiusBR) {
    radiusL++;radiusR++;radiusBL++;radiusBR++;
    DrawRectangleNoCorners(x, y, width, height, color, radiusL, radiusR, radiusBL, radiusBR);
    DrawCircle(x+radiusL, y+radiusL, radiusL, color, true);
    DrawCircle(x+width-radiusR-1, y+radiusR, radiusR, color, true);
    DrawCircle(x+radiusBL, y+height-radiusBL-1, radiusBL, color, true);
    DrawCircle(x+width-radiusBR-1, y+height-radiusBR-1, radiusBR, color, true);
}

unsigned int Graphics::GetWidth() {
    return framebuffer->Width;
}

unsigned int Graphics::GetHeight() {
    return framebuffer->Height;
}

void* Graphics::GetBaseAddress() {
    return framebuffer->BaseAddress;
}

unsigned int Graphics::GetPPSL() {
    return framebuffer->PixelsPerScanLine;
}

size_t Graphics::GetSize() {
    return framebuffer->Size;
}

void Graphics::test() {
    printf("Hello, world!");
}