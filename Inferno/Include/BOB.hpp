#ifndef BOB
#include <Framebuffer.hpp>

struct MemoryDescriptor {
  unsigned int Type;
  unsigned long long PhysicalStart, VirtualStart, NumberOfPages, Attribute;
};

struct BOB {
	unsigned long long DescriptorSize, MapSize;
	MemoryDescriptor* MemoryMap;

  Framebuffer* framebuffer;
};

#endif /* BOB */