#include "Drivers/Graphics/GOP/GOP.hpp"
#include "Graphics/Font/Font.hpp"
#include "Drivers/Memory/Memory.hpp"

extern uint64_t InfernoStart;
extern uint64_t InfernoEnd;

void putpixel(unsigned int x, unsigned int y, unsigned int color) {
  *(unsigned int*)((unsigned int*)framebuffer->Address + x + (y * framebuffer->PPSL)) = 0;
}

void main(Framebuffer* framebuffer, PSFFont* font, Memory* memory) {
  SetGlobalFramebuffer(framebuffer);
  SetGlobalFont(font);
  uint64_t MapEntries = memory->MapSize / memory->DescriptorSize;
  
  PageFrameAllocator newAllocator;
  newAllocator.ReadMemoryMap(memory->Map, memory->MapSize, memory->DescriptorSize);

  
  uint64_t kernelSize = (uint64_t)&InfernoEnd-(uint64_t)&InfernoStart;
  uint64_t kernelPages = (uint64_t)kernelSize/4096+1;
  newAllocator.LockPages(&InfernoStart, kernelPages);

  printf("Free: %d MB\n", newAllocator.GetFreeMem() / 1024 / 1024);
  printf("Used: %d MB\n", newAllocator.GetUsedMem() / 1024 / 1024);
  printf("Reserved: %d MB\n", newAllocator.GetReservedMem() / 1024 / 1024);
  for (int t=0;t<20;t++) {
    void* address = newAllocator.RequestPage();
    printf("%x\n", (int64_t)address);
  }
  while(1);
}