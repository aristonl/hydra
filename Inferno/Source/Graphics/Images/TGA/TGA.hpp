#pragma once

struct TGAHeader {
	unsigned char	id_length;
	unsigned char	color_map_type;
	unsigned char	image_type;
	unsigned short color_map_index;
	unsigned short color_map_length;
	unsigned char	depth;
	unsigned short xorg;
	unsigned short yorg;
	unsigned short width;
	unsigned short height;
	unsigned char	bbp;
	unsigned char	descriptor;
} __attribute__((packed));

struct TGAImage {
	struct TGAHeader*	header_ptr;
	void* buffer;
};