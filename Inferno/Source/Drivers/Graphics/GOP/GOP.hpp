#pragma once

class Framebuffer {
  public:
    Framebuffer(void* Address, unsigned long long Size, unsigned int Width, unsigned int Height, unsigned int PPSL) {
      this->Address = Address;
      this->Size = Size;
      this->Width = Width;
      this->Height = Height;
      this->PPSL = PPSL;
    }
    void* Address;
    unsigned long long Size;
    unsigned int Width, Height, PPSL;
};

void SetGlobalFramebuffer(Framebuffer* temp);

extern Framebuffer* framebuffer;