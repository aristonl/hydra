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