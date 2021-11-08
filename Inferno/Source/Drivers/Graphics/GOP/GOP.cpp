#include "GOP.hpp"

Framebuffer* framebuffer;
uint8_t* backbuffer;

void SetGlobalFramebuffer(Framebuffer* temp) {
  framebuffer = temp;
}

void putpixel(unsigned int x, unsigned int y, unsigned int color) {
  backbuffer[(4*framebuffer->PPSL*y+4*x)] = (color & 0xFF);
  backbuffer[(4*framebuffer->PPSL*y+4*x)+1] = (color & 0xFF00) >> 8;
  backbuffer[(4*framebuffer->PPSL*y+4*x)+2] = (color & 0xFF0000) >> 16;
  backbuffer[(4*framebuffer->PPSL*y+4*x)+3] = (color & 0xFF000000) >> 24;
}

void SwapBuffers() {
  memcpy(framebuffer->Address, backbuffer, framebuffer->Width * framebuffer->Height * 4);
}

void ClearBuffer() {
  memset(backbuffer, 0, framebuffer->Width * framebuffer->Height * 4);
  CursorX = 0;CursorY = 0;
}