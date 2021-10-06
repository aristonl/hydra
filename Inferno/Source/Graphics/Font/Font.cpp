#include "Font.hpp"
#include "../../Drivers/Graphics/GOP/GOP.hpp"

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

void printf(const char* str) {
  char* chr = (char*)str;
  while(*chr != 0){
    putc(*chr, CursorX, CursorY);
    CursorX+=8;
    if(CursorX + 8 > framebuffer->Width) {
      CursorX = 0;
      CursorY += 16;
    }
    chr++;
  }
}