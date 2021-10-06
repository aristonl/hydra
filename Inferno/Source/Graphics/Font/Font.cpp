#include "Font.hpp"
#include "../../Drivers/Graphics/GOP/GOP.hpp"
#include "string.hpp"
#include "../../../../Libraries/LibC/stdarg.h"

PSFFont* font;

void SetGlobalFont(PSFFont* temp) {
  font = temp;
}

unsigned int CursorX = 0, CursorY = 0;

void putc(char c, unsigned int xOff, unsigned int yOff) {
  char* fontPointer = (char*)font->GlyphBuffer + (c*font->Header->charSize);
  for (unsigned long y=yOff;y<yOff+16;y++) {
    for (unsigned long x=xOff;x<xOff+8;x++) {
      if ((*fontPointer & (0b10000000 >> (x - xOff))) > 0) {
        *(unsigned int*)((unsigned int*)framebuffer->Address + x + (y * framebuffer->PPSL)) = 0xFFFFFF;
      }
    }
    fontPointer++;
  }
}

void printf(const char* format, ...) {
  va_list args;
  va_start(args, format);

  char* chr = (char*)format;
  while(*chr != 0) {
    if (CursorX > framebuffer->Width) {
      CursorX=0;
      CursorY+=16;
    } else if (*chr == '\n') {
      CursorY+=16;
      CursorX=0;
    } else if (*chr == '%') {
      chr++;
      if (*chr == 'd') {
        unsigned int i = va_arg(args, int);
        printf(i);
      } else if (*chr == 'c') {
        unsigned int i = va_arg(args, int);
        putc(i, CursorX, CursorY);
        CursorX+=8;
      } else if (*chr == 'x') {
        unsigned int i = va_arg(args, int);
        hprintf(i);
      }
    } else {
      putc(*chr, CursorX, CursorY);
      CursorX+=8;
    }
    chr++;
  }
}

void printf(long long int i) {
  char* chr = (char*)to_string(i);
  while(*chr != 0) {
    if (CursorX > framebuffer->Width) {
      CursorX=0;
      CursorY+=16;
    } else if (*chr == '\n') {
      CursorY+=16;
      CursorX=0;
    } else {
      putc(*chr, CursorX, CursorY);
      CursorX+=8;
      if(CursorX + 8 > framebuffer->Width) {
        CursorX = 0;
        CursorY += 16;
      }
    }
    chr++;
  }
}

void hprintf(unsigned long long int i) {
  char* chr = (char*)to_hstring(i);
  while(*chr != 0) {
    if (CursorX > framebuffer->Width) {
      CursorX=0;
      CursorY+=16;
    } else if (*chr == '\n') {
      CursorY+=16;
      CursorX=0;
    } else {
      putc(*chr, CursorX, CursorY);
      CursorX+=8;
      if(CursorX + 8 > framebuffer->Width) {
        CursorX = 0;
        CursorY += 16;
      }
    }
    chr++;
  }
}