#include "Drivers/Graphics/GOP/GOP.hpp"
#include "Graphics/Font/Font.hpp"
#include "Drivers/Memory/Memory.hpp"

void putpixel(unsigned int x, unsigned int y, unsigned int color) {
  *(unsigned int*)((unsigned int*)framebuffer->Address + x + (y * framebuffer->PPSL)) = 0;
}

void main(Framebuffer* framebuffer, PSFFont* font, Memory* memory) {
  SetGlobalFramebuffer(framebuffer);
  SetGlobalFont(font);

  // uint64_t MapEntries = memory->MapSize / memory->DescriptorSize;
  // for (int i=0;i<MapEntries;i++) {
  //   MemoryDescriptor* desc = (MemoryDescriptor*)((uint64_t)memory->Map+(i*memory->DescriptorSize));
  //   printf(MemoryTypesStrings[desc->type]);
  //   printf(" %d KB\n", desc->numPages*4096/1024);
  // }
  while(1);
}



  // unsigned int* pixel = (unsigned int*)framebuffer->Address;
  // for (unsigned int y=0;y<=framebuffer->Height;y++) {
  //   for (unsigned int x=0;x<=framebuffer->Width;x++) {
  //     *(unsigned int*)(pixel + x + (y * framebuffer->PPSL)) = 0;
  //   }
  // }