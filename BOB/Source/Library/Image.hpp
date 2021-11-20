#ifndef TGAHeader

struct TGAHeader {
	unsigned char id_length, color_map_type, image_type;
	unsigned short color_map_index, color_map_length;
	unsigned char depth;
	unsigned short xorg, yorg, width, height;
	unsigned char bbp, descriptor;
} __attribute__((packed));

#endif

#ifndef TGAImage

struct TGAImage {
  struct TGAHeader* header_ptr;
  void* buffer;
};

#endif

#ifndef PrintTGA

void PrintTGA(struct TGAImage* image, struct SystemTable* SystemTable, Framebuffer framebuffer, unsigned int x0, unsigned int y0) {
  unsigned int* img = (unsigned int*)image->buffer;
  unsigned int height = image->header_ptr->height;
  unsigned int width = image->header_ptr->width;
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