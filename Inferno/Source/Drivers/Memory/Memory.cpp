#include "Memory.hpp"

const char* MemoryTypesStrings[] {
  "EfiReservedMemoryType",
  "EfiLoaderCode",
  "EfiLoaderData",
  "EfiBootServicesCode",
  "EfiBootServicesData",
  "EfiRuntimeServicesCode",
  "EfiRuntimeServicesData",
  "EfiConventionalMemory",
  "EfiUnusableMemory",
  "EfiACPIReclaimMemory",
  "EfiACPIMemoryNVS",
  "EfiMemoryMappedIO",
  "EfiMemoryMappedIOPortSpace",
  "EfiPalCode"
};

uint64_t GetMemorySize(MemoryDescriptor* Map, uint64_t MapEntries, uint64_t DescriptorSize) {
  static uint64_t memorySizeBytes = 0;
  if (memorySizeBytes>0) return memorySizeBytes;
  for (int i=0;i<MapEntries;i++) {
    MemoryDescriptor* desc = (MemoryDescriptor*)((uint64_t)Map+(i*DescriptorSize));
    memorySizeBytes+=desc->numPages*4096;
  }
  return memorySizeBytes;
}

bool Bitmap::operator[](uint64_t index) {
  uint64_t byteIndex = index/8;
  uint8_t bitIndex = index%8;
  uint8_t bitIndexer = 0b10000000 >> bitIndex;
  if ((Buffer[byteIndex] & bitIndexer) > 0) return true;
  return false;
}

void Bitmap::Set(uint64_t index, bool value) {
  uint64_t byteIndex = index/8;
  uint8_t bitIndex = index%8;
  uint8_t bitIndexer = 0b10000000 >> bitIndex;
  Buffer[byteIndex] &= ~bitIndexer;
  if (value) Buffer[byteIndex] |= bitIndexer;
}

uint64_t freeMemory;
uint64_t reservedMemory;
uint64_t usedMemory;

bool Initialized = false;

void PageFrameAllocator::ReadMemoryMap(MemoryDescriptor* Map, size_t MapSize, size_t DescriptorSize) {
  if (Initialized) return;
  Initialized = true;
  uint64_t MapEntries = MapSize/DescriptorSize;
  void* largestFreeMemorySegment = (void*)0;
  size_t largestFreeMemorySegmentSize = 0;
  for (int i=0;i<MapEntries;i++) {
    MemoryDescriptor* desc = (MemoryDescriptor*)((uint64_t)Map+(i*DescriptorSize));
    if (desc->type == 7) {
      if (desc->numPages*4096 > largestFreeMemorySegmentSize) {
        largestFreeMemorySegment = desc->physAddr;
        largestFreeMemorySegmentSize = desc->numPages*4096;
      }
    }
  }
  uint64_t memorySize = GetMemorySize(Map, MapEntries, DescriptorSize);
  freeMemory = memorySize;
  uint64_t bitmapSize = memorySize/4096/8+1;
  InitBitmap(bitmapSize, largestFreeMemorySegment);
  
}

void PageFrameAllocator::InitBitmap(size_t bitmapSize, void* BufferAddress) {
  PageBitmap.Size = bitmapSize;
  PageBitmap.Buffer = (uint8_t*)BufferAddress;
  for (int i=0;i<bitmapSize;i++) *(uint8_t*)(PageBitmap.Buffer+i)=0;
}