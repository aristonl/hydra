#pragma once
#include "../../../../std/Types.h"

struct MemoryDescriptor {
  uint32_t type;
  void* physAddr, *virtAddr;
  uint64_t numPages, attribs;
};

typedef struct {
  MemoryDescriptor* Map;
  uint64_t DescriptorSize, MapSize;
} Memory;

uint64_t GetMemorySize(MemoryDescriptor* Map, uint64_t MapEntries, uint64_t DescriptorSize);

class Bitmap {
  public:
    size_t Size;
    uint8_t* Buffer;
    bool operator[](uint64_t index);
    void Set(uint64_t index, bool value);
};

extern const char* MemoryTypesStrings[];