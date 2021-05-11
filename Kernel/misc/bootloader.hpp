/*
 * Copyright Null LLC
 * Please read the License!
 *  _     _           _ 
 * | |   | |         | |
 * | |__ | |_   _  _ | | ____ ____
 * |  __)| | | | |/ || |/ ___) _  |     bootloader.hpp
 * | |   | | |_| ( (_| | |  ( ( | |     Contains structs required for passing data from bootloader to kernel.
 * |_|   |_|\__  |\____|_|   \_||_|
 *         (____/
 */

#pragma once
#include <stddef.h>

struct EFI_MEMORY_DESCRIPTOR {
    uint32_t type;
    void* physAddr;
    void* virtAddr; 
    uint64_t numPages;
    uint64_t attribs;
};

struct Framebuffer {
	void* BaseAddress;
	size_t Size;
	unsigned int Width, Height, PixelsPerScanLine;
	int maxMode, nativeMode, version;
};

struct PSF1_HEADER {
	unsigned char magic[2];
	unsigned char mode;
	unsigned char charsize;
};

struct PSF1_FONT {
	PSF1_HEADER* psf1_Header;
	void* glyphBuffer;
};

struct BootData {
	struct Framebuffer* framebuffer;
	struct PSF1_FONT* font;
	EFI_MEMORY_DESCRIPTOR* Map;
	uint64_t MapSize, MapDescriptorSize;
};