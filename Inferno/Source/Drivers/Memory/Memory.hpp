#pragma once
#include "../../../../std/Types.h"

struct MemoryDescriptor {
  uint32_t type;
  void* physAddr, *virtAddr;
  uint64_t numPages, attribs;
};

typedef struct {
  MemoryDescriptor* Map;
  unsigned long long DescriptorSize, MapSize;
} Memory;

uint64_t GetMemorySize(MemoryDescriptor* Map, uint64_t MapEntries, uint64_t DescriptorSize);

extern const char* MemoryTypesStrings[];