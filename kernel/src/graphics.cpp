#include "graphics.h"
#include "string.h"

void GUI::SetFont(PSF1_FONT* newFont) {
    font = newFont;
}

void GUI::SetColor(unsigned int newColor) {
    color = newColor;
}

void GUI::SetFramebuffer(Framebuffer* newFramebuffer) {
    framebuffer = newFramebuffer;
}

void GUI::putpixel(int x, int y, unsigned int color) {
    y+=1;
    unsigned int* pixPtr = (unsigned int*) framebuffer->BaseAddress;
    *(unsigned int*)(pixPtr + x + (y * framebuffer->PixelsPerScanLine)) = color;
}

void GUI::DrawRectangle(int x, int y, int width, int height, unsigned int color) {
    for (int j = y; j < y+height; j++) {
        for (int i = x; i < x+width; i++) {
            putpixel(i,j,color);
        }
    }
}

void GUI::DrawRectangleFromTo(int x, int y, int toX, int toY, unsigned int color) {
    for (int j = y; j < toY; j++) {
        for (int i = x; i < toX; i++) {
            putpixel(i,j,color);
        }
    }
}

void GUI::DrawCursor(int x, int y, int64_t style) {
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
}

void GUI::putchar(char c, unsigned int xOff, unsigned int yOff) {
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

void GUI::printf(const char* str) {
    char* chr = (char*)str;
    while(*chr != 0){
        putchar(*chr, TextCursorPosition.X, TextCursorPosition.Y);
        TextCursorPosition.X+=8;
        if(TextCursorPosition.X + 8 > framebuffer->Width)
        {
            TextCursorPosition.X = 0;
            TextCursorPosition.Y += 16;
        }
        chr++;
    }
}

void GUI::SetX(unsigned int x) {
    TextCursorPosition.X = x;
}

void GUI::SetY(unsigned int y) {
    TextCursorPosition.Y = y;
}

unsigned int GUI::GetX() {
    return TextCursorPosition.X;
}

unsigned int GUI::GetY() {
    return TextCursorPosition.Y;
}

void GUI::SetXY(unsigned int x, unsigned int y) {
    TextCursorPosition.X = x;
    TextCursorPosition.Y = y;
}

void GUI::DrawCircle(int x0, int y0, int radius, unsigned int color, bool filled) {
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

void GUI::DrawRectangleNoCorners(int x, int y, int width, int height, unsigned int color, int radius) {
    for (int j = y; j < y+height; j++) {
        for (int i = x; i < x+width; i++) {
            if (i < x+radius && j < y+radius) {}
            else if (i > x+width-radius && j < y+radius) {}
            else if (i < x+radius && j > y+height-radius) {}
            else if (i > x+width-radius && j > y+height-radius) {}
            else putpixel(i,j,color);
        }
    }
}

void GUI::DrawBox(int x, int y, int width, int height, unsigned int color, int radius) {
    radius++;
    DrawRectangleNoCorners(x, y, width, height, color, radius);
    DrawCircle(x+radius, y+radius, radius, color, true);
    DrawCircle(x+width-radius-1, y+radius, radius, color, true);
    DrawCircle(x+radius, y+height-radius-1, radius, color, true);
    DrawCircle(x+width-radius-1, y+height-radius-1, radius, color, true);
}