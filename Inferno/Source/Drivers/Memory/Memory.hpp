#pragma once

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

class PageFrameAllocator {
  public:
    void ReadMemoryMap(MemoryDescriptor* Map, size_t MapSize, size_t DescriptorSize);
    Bitmap PageBitmap;
    void FreePage(void* address);
    void LockPage(void* address);
    void FreePages(void* address, uint64_t pageCount);
    void LockPages(void* address, uint64_t pageCount);
    uint64_t GetFreeMem();
    uint64_t GetUsedMem();
    uint64_t GetReservedMem();
  private:
    void InitBitmap(size_t bitmapSize, void* BufferAddress);
    void ReservedPage(void* address);
    void UnreservedPage(void* address);
    void ReservedPages(void* address, uint64_t pageCount);
    void UnreservedPages(void* address, uint64_t pageCount);
};

extern const char* MemoryTypesStrings[];