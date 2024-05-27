#ifndef TGAHeader

struct TGAHeader {
	unsigned char IDSize, ColorMapType, ImageType;
	unsigned short ColorMapIndex, ColorMapSize;
	unsigned char Depth;
	unsigned short X, Y, Width, Height;
	unsigned char BytesPerPixel, Descriptor;
} __attribute__((packed));

#endif

#ifndef TGAImage

struct TGAImage {
  struct TGAHeader* Pointer;
  void* Buffer;
};

#endif

#ifndef PrintTGA

void PrintTGA(struct TGAImage* image, struct SystemTable* SystemTable, Framebuffer framebuffer, unsigned int x0, unsigned int y0) {
  unsigned int* img = (unsigned int*)image->Buffer;
  unsigned int height = image->Pointer->Height;
  unsigned int width = image->Pointer->Width;
  for (unsigned long long dy=0;dy<height;dy++) {
    for (unsigned long long dx=0;dx<width;dx++) {
      unsigned long long offset = dx+(height*dy);
      unsigned int color = *(img+offset);
      unsigned long long x = dx+(x0)-(width/2);
      unsigned long long y = dy+(y0)-(height/2);
			*(unsigned int*)((unsigned int*)framebuffer.Address + x + (y * framebuffer.PPSL)) = color;
    }
  }
}

#endif