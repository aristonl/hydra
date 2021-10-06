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
  uint64_t MapEntries = MapSize / DescriptorSize;
  void *largestFreeMemorySegment = (void *) 0;
  size_t largestFreeMemorySegmentSize = 0;
  for (int i = 0; i < MapEntries; i++) {
    MemoryDescriptor *desc = (MemoryDescriptor *) ((uint64_t) Map + (i * DescriptorSize));
    if (desc->type == 7) {
      if (desc->numPages * 4096 > largestFreeMemorySegmentSize) {
        largestFreeMemorySegment = desc->physAddr;
        largestFreeMemorySegmentSize = desc->numPages * 4096;
      }
    }
  }
  uint64_t memorySize = GetMemorySize(Map, MapEntries, DescriptorSize);
  freeMemory = memorySize;
  uint64_t bitmapSize = memorySize / 4096 / 8 + 1;
  InitBitmap(bitmapSize, largestFreeMemorySegment);
  LockPages(&PageBitmap, PageBitmap.Size / 4096 + 1);
  for (int i = 0; i < MapEntries; i++) {
    MemoryDescriptor* desc = (MemoryDescriptor*)((uint64_t) Map + (i * DescriptorSize));
    if (desc->type != 7) ReservePages(desc->physAddr, desc->numPages);
  }
}

void PageFrameAllocator::InitBitmap(size_t bitmapSize, void* BufferAddress) {
  PageBitmap.Size = bitmapSize;
  PageBitmap.Buffer = (uint8_t*)BufferAddress;
  for (int i=0;i<bitmapSize;i++) *(uint8_t*)(PageBitmap.Buffer+i)=0;
}

void PageFrameAllocator::FreePage(void *address) {
  uint64_t index = (uint64_t)address / 4096;
  if (PageBitmap[index] == false) return;
  PageBitmap.Set(index, false);
  freeMemory += 4096;
  usedMemory -= 4096;
}

void PageFrameAllocator::FreePages(void *address, uint64_t pageCount) {
  for (int t = 0; t < pageCount; t++) FreePage((void*)((uint64_t)address + (t * 4096)));
}

void PageFrameAllocator::LockPage(void *address) {
  uint64_t index = (uint64_t)address / 4096;
  if (PageBitmap[index] == true) return;
  PageBitmap.Set(index, true);
  freeMemory -= 4096;
  usedMemory += 4096;
}

void PageFrameAllocator::LockPages(void *address, uint64_t pageCount) {
  for (int t = 0; t < pageCount; t++) LockPage((void*)((uint64_t)address + (t * 4096)));
}

void PageFrameAllocator::UnreservePage(void *address) {
  uint64_t index = (uint64_t)address / 4096;
  if (PageBitmap[index] == false) return;
  PageBitmap.Set(index, false);
  freeMemory += 4096;
  reservedMemory -= 4096;
}

void PageFrameAllocator::UnreservePages(void *address, uint64_t pageCount) {
  for (int t = 0; t < pageCount; t++) UnreservePage((void*)((uint64_t)address + (t * 4096)));
}

void PageFrameAllocator::ReservePage(void *address) {
  uint64_t index = (uint64_t)address / 4096;
  if (PageBitmap[index] == true) return;
  PageBitmap.Set(index, true);
  freeMemory -= 4096;
  reservedMemory += 4096;
}

void PageFrameAllocator::ReservePages(void *address, uint64_t pageCount) {
  for (int t = 0; t < pageCount; t++) ReservePage((void*)((uint64_t)address + (t * 4096)));
}

void* PageFrameAllocator::RequestPage() {
  for (uint64_t index=0;index<PageBitmap.Size*8;index++) {
    if (PageBitmap[index]==true) continue;
    LockPage((void*)(index*4096));
    return (void*)(index*4096);
  }
  return (void*)0; // Come back to here when ahci driver is done.
}

uint64_t PageFrameAllocator::GetFreeMem() {
  return freeMemory;
}
uint64_t PageFrameAllocator::GetUsedMem() {
  return usedMemory;
}
uint64_t PageFrameAllocator::GetReservedMem() {
  return reservedMemory;
}