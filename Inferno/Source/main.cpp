typedef unsigned long long size_t;

class Framebuffer {
  public:
    Framebuffer(void* Address, size_t Size, unsigned int Width, unsigned int Height, unsigned int PPSL) {
      this->Address = Address;
      this->Size = Size;
      this->Width = Width;
      this->Height = Height;
      this->PPSL = PPSL;
    }
    void* Address;
    size_t Size;
    unsigned int Width, Height, PPSL;
};

void main(Framebuffer* framebuffer) {
  // unsigned int* pixel = (unsigned int*)framebuffer->Address;
  // for (unsigned int y=0;y<=framebuffer->Height;y++) {
  //   for (unsigned int x=0;x<=framebuffer->Width;x++) {
  //     *(unsigned int*)(pixel + x + (y * framebuffer->PPSL)) = 0;
  //   }
  // }
  while(1);
}