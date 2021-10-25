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
void printf(const char* string, ...);
void print8h(unsigned char value);
void printf(long long int i);
void hprintf(unsigned int i);

extern PSFFont* font;
extern unsigned int CursorX, CursorY;