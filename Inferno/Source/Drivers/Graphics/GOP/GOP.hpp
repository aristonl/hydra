#pragma once
#include "../../Memory/Memory.hpp"
#include "../../../Graphics/Font/Font.hpp"

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

void putpixel(unsigned int x, unsigned int y, unsigned int color);
void SwapBuffers();
void ClearBuffer();

extern Framebuffer* framebuffer;
extern uint8_t* backbuffer;