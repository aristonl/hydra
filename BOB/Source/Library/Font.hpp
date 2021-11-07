#ifndef PSFHeader

struct PSFHeader {
  unsigned char magic[2], mode, charSize;
};

#endif

#ifndef PSFFont

struct PSFFont {
  PSFHeader* Header;
  void* GlyphBuffer;
};

#endif