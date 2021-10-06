#include "Drivers/Graphics/GOP/GOP.hpp"
#include "Graphics/Font/Font.hpp"

void main(Framebuffer* framebuffer, PSFFont* font) {
  // unsigned int* pixel = (unsigned int*)framebuffer->Address;
  // for (unsigned int y=0;y<=framebuffer->Height;y++) {
  //   for (unsigned int x=0;x<=framebuffer->Width;x++) {
  //     *(unsigned int*)(pixel + x + (y * framebuffer->PPSL)) = 0;
  //   }
  // }
  SetGlobalFramebuffer(framebuffer);
  SetGlobalFont(font);
  printf("Hello, world!");
  while(1);
}