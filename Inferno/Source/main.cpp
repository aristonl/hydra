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
  while(1);
}