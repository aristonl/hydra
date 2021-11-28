#include "Font.hpp"
#include "../../Drivers/Graphics/GOP/GOP.hpp"
#include "../../../../Libraries/LibC/stdarg.h"
#include "../../Drivers/Memory/Memory.hpp"

PSFFont* font;

void SetGlobalFont(PSFFont* temp) {
  font = temp;
}

unsigned int CursorX=0;
unsigned int CursorY=0;

void putc(char c, unsigned int xOff, unsigned int yOff) {
  char* fontPointer = (char*)font->GlyphBuffer + (c*font->Header->charSize);
  for (unsigned long y=yOff;y<yOff+16;y++) {
    for (unsigned long x=xOff;x<xOff+8;x++) {
      if ((*fontPointer & (0b10000000 >> (x - xOff))) > 0) {
        *(unsigned int*)((unsigned int*)framebuffer->Address + x + (y * framebuffer->PPSL)) = 0xFFFFFF;
      }
    }
    fontPointer++;
  }
}

void printf(char c) {
  putc(c, CursorX, CursorY);
  CursorX+=8;
  if (CursorX >= framebuffer->Width-8) {
    CursorX=0;
    CursorY+=16;
  } else if (CursorY > framebuffer->Height-16) {
    CursorY=0;
    CursorX=0;
    memset(framebuffer->Address, 0, framebuffer->Size);
  }
}

void printf(const char* format, ...) {
  va_list args;
  va_start(args, format);

  char* chr = (char*)format;
  while(*chr != 0) {
    if (*chr == '\b') {
      if (CursorX == 0 && CursorY >= 16) {
        CursorX=framebuffer->Width-8;
        CursorY-=16;
        for (unsigned int x=CursorX;x<CursorX+8;x++) {
          for (unsigned int y=CursorY;y<CursorY+16;y++) {
            *(unsigned int*)((unsigned int*)framebuffer->Address + x + (y * framebuffer->PPSL)) = 0;
          }
        }
      } else {
        CursorX-=8;
        for (unsigned int x=CursorX;x<CursorX+8;x++) {
          for (unsigned int y=CursorY;y<CursorY+16;y++) {
            *(unsigned int*)((unsigned int*)framebuffer->Address + x + (y * framebuffer->PPSL)) = 0;
          }
        }
      }
    } else if (*chr == '\t') {
      printf("    ");
    } else if (CursorX >= framebuffer->Width-8) {
      CursorX=0;
      CursorY+=16;
    } else if (CursorY > framebuffer->Height-16) {
      CursorY=0;
      CursorX=0;
      memset(framebuffer->Address, 0, framebuffer->Size);
    } else if (*chr == '\n') {
      CursorY+=16;
      CursorX=0;
    } else if (*chr == '%') {
      chr++;
      if (*chr == 'd') {
        unsigned int i = va_arg(args, int);
        printf((long long) i);
      } else if (*chr == 'c') {
        unsigned int i = va_arg(args, int);
        putc(i, CursorX, CursorY);
        CursorX+=8;
      } else if (*chr == 'x') {
        unsigned int i = va_arg(args, int);
        hprintf(i);
      }
    } else {
      putc(*chr, CursorX, CursorY);
      CursorX+=8;
    }
    chr++;
  }
}

void print8h(unsigned char value) {
  printf(to_hstring(value));
}

void printf(long long int i) {
  char* chr = (char*)to_string(i);
  while(*chr != 0) {
    if (CursorX > framebuffer->Width) {
      CursorX=0;
      CursorY+=16;
    } else if (CursorY > framebuffer->Height-16) {
      CursorY=0;
      CursorX=0;
      memset(framebuffer->Address, 0, framebuffer->Size);
    } else if (*chr == '\n') {
      CursorY+=16;
      CursorX=0;
    } else {
      putc(*chr, CursorX, CursorY);
      CursorX+=8;
      if(CursorX + 8 > framebuffer->Width) {
        CursorX = 0;
        CursorY += 16;
      }
    }
    chr++;
  }
}

void hprintf(unsigned int i) {
  char* chr = (char*)to_hstring(i);
  while(*chr != 0) {
    if (CursorX > framebuffer->Width) {
      CursorX=0;
      CursorY+=16;
    } else if (CursorY > framebuffer->Height-16) {
      CursorY=0;
      CursorX=0;
      memset(framebuffer->Address, 0, framebuffer->Size);
    } else if (*chr == '\n') {
      CursorY+=16;
      CursorX=0;
    } else {
      putc(*chr, CursorX, CursorY);
      CursorX+=8;
      if(CursorX + 8 > framebuffer->Width) {
        CursorX = 0;
        CursorY += 16;
      }
    }
    chr++;
  }
}

char uintTo_StringOutput[128];
const char* to_string(uint64_t value) {
  uint8_t size;
  uint64_t sizeTest = value;
  while (sizeTest / 10 > 0) {
    sizeTest /= 10;
    size++;
  }
  uint8_t index = 0;
  while(value / 10 > 0) {
    uint8_t remainder = value % 10;
    value /= 10;
    uintTo_StringOutput[size - index] = remainder + '0';
    index++;
  }
  uint8_t remainder = value % 10;
  uintTo_StringOutput[size - index] = remainder + '0';
  uintTo_StringOutput[size + 1] = 0; 
  return uintTo_StringOutput;
}

char hexTo_StringOutput[128];
const char* to_hstring(uint64_t value) {
  uint64_t* valPtr = &value;
  uint8_t* ptr;
  uint8_t tmp;
  uint8_t size = 8 * 2 - 1;
  for (uint8_t i = 0; i < size; i++) {
    ptr = ((uint8_t*)valPtr + i);
    tmp = ((*ptr & 0xF0) >> 4);
    hexTo_StringOutput[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
    tmp = ((*ptr & 0x0F));
    hexTo_StringOutput[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
  }
  hexTo_StringOutput[size + 1] = 0;
  return hexTo_StringOutput;
}

char hexTo_StringOutput32[128];
const char* to_hstring(uint32_t value) {
  uint32_t* valPtr = &value;
  uint8_t* ptr;
  uint8_t tmp;
  uint8_t size = 4 * 2 - 1;
  for (uint8_t i = 0; i < size; i++) {
    ptr = ((uint8_t*)valPtr + i);
    tmp = ((*ptr & 0xF0) >> 4);
    hexTo_StringOutput32[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
    tmp = ((*ptr & 0x0F));
    hexTo_StringOutput32[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
  }
  hexTo_StringOutput32[size + 1] = 0;
  return hexTo_StringOutput32;
}

char hexTo_StringOutput16[128];
const char* to_hstring(uint16_t value) {
  uint16_t* valPtr = &value;
  uint8_t* ptr;
  uint8_t tmp;
  uint8_t size = 2 * 2 - 1;
  for (uint8_t i = 0; i < size; i++) {
    ptr = ((uint8_t*)valPtr + i);
    tmp = ((*ptr & 0xF0) >> 4);
    hexTo_StringOutput16[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
    tmp = ((*ptr & 0x0F));
    hexTo_StringOutput16[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
  }
  hexTo_StringOutput16[size + 1] = 0;
  return hexTo_StringOutput16;
}

char hexTo_StringOutput8[128];
const char* to_hstring(uint8_t value) {
  uint8_t* valPtr = &value;
  uint8_t* ptr;
  uint8_t tmp;
  uint8_t size = 1 * 2 - 1;
  for (uint8_t i = 0; i < size; i++) {
    ptr = ((uint8_t*)valPtr + i);
    tmp = ((*ptr & 0xF0) >> 4);
    hexTo_StringOutput8[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
    tmp = ((*ptr & 0x0F));
    hexTo_StringOutput8[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
  }
  hexTo_StringOutput8[size + 1] = 0;
  return hexTo_StringOutput8;
}

char intTo_StringOutput[128];
const char* to_string(int64_t value) {
  uint8_t isNegative = 0;
  if (value < 0){
    isNegative = 1;
    value *= -1;
    intTo_StringOutput[0] = '-';
  }
  uint8_t size;
  uint64_t sizeTest = value;
  while (sizeTest / 10 > 0) {
    sizeTest /= 10;
    size++;
  }
  uint8_t index = 0;
  while(value / 10 > 0) {
    uint8_t remainder = value % 10;
    value /= 10;
    intTo_StringOutput[isNegative + size - index] = remainder + '0';
    index++;
  }
  uint8_t remainder = value % 10;
  intTo_StringOutput[isNegative + size - index] = remainder + '0';
  intTo_StringOutput[isNegative + size + 1] = 0; 
  return intTo_StringOutput;
}