typedef unsigned long long size_t;

typedef struct Framebuffer {
  void* Address;
  size_t Size;
  unsigned int Width, Height, PPSL;
} Framebuffer;

int main(Framebuffer* framebuffer) {
  unsigned int* pixPtr = (unsigned int*)framebuffer->Address;
  for (unsigned int x=0;x<=framebuffer->Width;x++) {
    for (unsigned int y=0;y<=framebuffer->Height;y++) {
      *(unsigned int*)(pixPtr + x + (y * framebuffer->PPSL)) = 0xFFFFFF;
    }
  }
  
  return 0;
}