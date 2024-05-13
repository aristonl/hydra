#ifndef PSFHeader

struct PSFHeader {
  unsigned char Magic[2], Mode, GlyphSize;
};

#endif

#ifndef PSFFont

struct PSFFont {
  PSFHeader* Header;
  void* GlyphBuffer;
};

#endif