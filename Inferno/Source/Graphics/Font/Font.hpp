#pragma once
#include "../../../../std/Types.h"

const char* to_string(uint64_t value);
const char* to_hstring(uint64_t value);
const char* to_hstring(uint32_t value);
const char* to_hstring(uint16_t value);
const char* to_hstring(uint8_t value);
const char* to_string(int64_t value);

typedef struct {
  unsigned char magic[2], mode, charSize;
} PSFHeader;

typedef struct {
  PSFHeader* Header;
  void* GlyphBuffer;
} PSFFont;

void SetGlobalFont(PSFFont* temp);

void putc(char c, unsigned int xOff, unsigned int yOff);
void printf(char c);
void printf(const char* string, ...);
void print8h(unsigned char value);
void printf(long long int i);
void hprintf(unsigned int i);

extern PSFFont* font;
extern unsigned int CursorX, CursorY;