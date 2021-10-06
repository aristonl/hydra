#pragma once
#include "../../../../std/Types.h"

typedef struct {
  void* Map;
  unsigned long long DescriptorSize, MapSize;
} Memory;

struct MemoryDescriptor {
  uint32_t type;
  void* physAddr, *virtAddr;
  uint64_t numPages, attribs;
};

extern const char* MemoryTypesStrings[];