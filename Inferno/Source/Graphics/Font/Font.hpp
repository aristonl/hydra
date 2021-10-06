#pragma once

typedef struct {
  unsigned char magic[2], mode, charSize;
} PSFHeader;

typedef struct {
  PSFHeader* Header;
  void* GlyphBuffer;
} PSFFont;

void SetGlobalFont(PSFFont* temp);

void putc(char c, unsigned int xOff, unsigned int yOff);
void printf(const char* string);

extern PSFFont* font;