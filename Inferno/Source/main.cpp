#include "Drivers/Graphics/GOP/GOP.hpp"
#include "Graphics/Font/Font.hpp"

void putpixel(unsigned int x, unsigned int y, unsigned int color) {
  *(unsigned int*)((unsigned int*)framebuffer->Address + x + (y * framebuffer->PPSL)) = 0;
}

void main(Framebuffer* framebuffer, PSFFont* font) {
  // unsigned int* pixel = (unsigned int*)framebuffer->Address;
  // for (unsigned int y=0;y<=framebuffer->Height;y++) {
  //   for (unsigned int x=0;x<=framebuffer->Width;x++) {
  //     *(unsigned int*)(pixel + x + (y * framebuffer->PPSL)) = 0;
  //   }
  // }
  SetGlobalFramebuffer(framebuffer);
  SetGlobalFont(font);
  printf("Hello, World!\nThis is a hexadecimal: %x", 0xFFaa88);
  printf("\nThis is a int: %d", 4664);
  printf("\nThis is a char: %c", 'A');
  while(1);
}