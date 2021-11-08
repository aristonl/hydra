#include "Memory.hpp"

extern "C" void __stack_chk_fail(void) {}

PageTableManager pageTableManager = 0;

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
  "EfiPalCode",
  "EfiMaxMemoryType"
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

void memset(void* start, uint8_t value, uint64_t num) {
  for (uint64_t i=0;i<num;i++) *(uint8_t*)((uint64_t)start+i)=value;
}

bool Bitmap::operator[](uint64_t index) {
  if (index >= Size*8) return false;
  uint64_t byteIndex = index/8;
  uint8_t bitIndex = index%8;
  uint8_t bitIndexer = 0b10000000 >> bitIndex;
  if ((Buffer[byteIndex] & bitIndexer) > 0) return true;
  return false;
}

bool Bitmap::Set(uint64_t index, bool value) {
  if (index >= Size*8) return false;
  uint64_t byteIndex = index/8;
  uint8_t bitIndex = index%8;
  uint8_t bitIndexer = 0b10000000 >> bitIndex;
  Buffer[byteIndex] &= ~bitIndexer;
  if (value) Buffer[byteIndex] |= bitIndexer;
  return true;
}

uint64_t freeMemory;
uint64_t reservedMemory;
uint64_t usedMemory;
PageFrameAllocator Allocator;

bool Initialized = false;

void PageFrameAllocator::ReadMemoryMap(MemoryDescriptor* Map, size_t MapSize, size_t DescriptorSize) {
  if (Initialized) return;
  Initialized = true;
  uint64_t MapEntries = MapSize / DescriptorSize;
  void *largestFreeMemorySegment = (void *) 0;
  size_t largestFreeMemorySegmentSize = 0;
  for (int i = 0; i < MapEntries; i++) {
    MemoryDescriptor *desc = (MemoryDescriptor*)((uint64_t)Map + (i * DescriptorSize));
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
  ReservePages(0, memorySize / 4096+1);
  for (int i = 0; i < MapEntries; i++) {
    MemoryDescriptor* desc = (MemoryDescriptor*)((uint64_t) Map + (i * DescriptorSize));
    if (desc->type == 7) UnreservePages(desc->physAddr, desc->numPages);
  }
  ReservePages(0, 0x1000);
  LockPages(PageBitmap.Buffer, PageBitmap.Size / 4096 + 1);
}

void PageFrameAllocator::InitBitmap(size_t bitmapSize, void* BufferAddress) {
  PageBitmap.Size = bitmapSize;
  PageBitmap.Buffer = (uint8_t*)BufferAddress;
  for (int i=0;i<bitmapSize;i++) *(uint8_t*)(PageBitmap.Buffer+i)=0;
}

uint64_t pageBitMapIndex=0;
void* PageFrameAllocator::RequestPage() {
  for (pageBitMapIndex;pageBitMapIndex<PageBitmap.Size*8;pageBitMapIndex++) {
    if (PageBitmap[pageBitMapIndex]==true) continue;
    LockPage((void*)(pageBitMapIndex*4096));
    return (void*)(pageBitMapIndex*4096);
  }
  return (void*)0; // Come back to here when ahci driver is done.
}

void PageFrameAllocator::FreePage(void *address) {
  uint64_t index = (uint64_t)address / 4096;
  if (PageBitmap[index] == false) return;
  if (PageBitmap.Set(index, false)) {
    freeMemory += 4096;
    usedMemory -= 4096;
    if (pageBitMapIndex>index)pageBitMapIndex=index;
  }
}

void PageFrameAllocator::FreePages(void *address, uint64_t pageCount) {
  for (int t = 0; t < pageCount; t++) FreePage((void*)((uint64_t)address + (t * 4096)));
}

void PageFrameAllocator::LockPage(void *address) {
  uint64_t index = (uint64_t)address / 4096;
  if (PageBitmap[index] == true) return;
  if (PageBitmap.Set(index, true)) {
    freeMemory -= 4096;
    usedMemory += 4096;
  }
}

void PageFrameAllocator::LockPages(void *address, uint64_t pageCount) {
  for (int t = 0; t < pageCount; t++) LockPage((void*)((uint64_t)address + (t * 4096)));
}

void PageFrameAllocator::UnreservePage(void *address) {
  uint64_t index = (uint64_t)address / 4096;
  if (PageBitmap[index] == false) return;
  if (PageBitmap.Set(index, false)) {
    freeMemory += 4096;
    reservedMemory -= 4096;
    if (pageBitMapIndex>index)pageBitMapIndex=index;
  }
}

void PageFrameAllocator::UnreservePages(void *address, uint64_t pageCount) {
  for (int t = 0; t < pageCount; t++) UnreservePage((void*)((uint64_t)address + (t * 4096)));
}

void PageFrameAllocator::ReservePage(void *address) {
  uint64_t index = (uint64_t)address / 4096;
  if (PageBitmap[index] == true) return;
  if (PageBitmap.Set(index, true)) {
    freeMemory -= 4096;
    reservedMemory += 4096;
  }
}

void PageFrameAllocator::ReservePages(void *address, uint64_t pageCount) {
  for (int t = 0; t < pageCount; t++) ReservePage((void*)((uint64_t)address + (t * 4096)));
}

uint64_t PageFrameAllocator::GetFreeMem() { return freeMemory; }
uint64_t PageFrameAllocator::GetUsedMem() { return usedMemory; }
uint64_t PageFrameAllocator::GetReservedMem() { return reservedMemory; }

void PageDirectoryEntry::SetFlag(PT_Flag flag, bool enabled){
  uint64_t bitSelector = (uint64_t)1 << flag;
  Value &= ~bitSelector;
  if (enabled){
    Value |= bitSelector;
  }
}

bool PageDirectoryEntry::GetFlag(PT_Flag flag){
  uint64_t bitSelector = (uint64_t)1 << flag;
  return Value & bitSelector;
}

uint64_t PageDirectoryEntry::GetAddress(){
  return (Value & 0x000ffffffffff000) >> 12;
}

void PageDirectoryEntry::SetAddress(uint64_t address){
  address &= 0x000000ffffffffff;
  Value &= 0xfff0000000000fff;
  Value |= (address << 12);
}

PageMapIndexer::PageMapIndexer(uint64_t virtualAddress) {
  virtualAddress >>= 12;
  this->P_i = virtualAddress&0x1ff;
  virtualAddress >>=9;
  this->PT_i = virtualAddress&0x1ff;
  virtualAddress >>=9;
  this->PD_i = virtualAddress&0x1ff;
  virtualAddress >>=9;
  this->PDP_i = virtualAddress&0x1ff;
}

PageTableManager::PageTableManager(PageTable* Level4Address) { this->Level4Address = Level4Address; }

void PageTableManager::MapMemory(void* virtualMemory, void* physicalMemory) {
  PageMapIndexer indexer = PageMapIndexer((uint64_t)virtualMemory);
  PageDirectoryEntry PDE;
  PDE=Level4Address->entries[indexer.PDP_i];
  PageTable* PDP;
  if (!PDE.GetFlag(PT_Flag::Present)) {
    PDP=(PageTable*)Allocator.RequestPage();
    memset(PDP, 0, 0x1000);
    PDE.SetAddress((uint64_t)PDP>>12);
    PDE.SetFlag(PT_Flag::Present, true);
    PDE.SetFlag(PT_Flag::ReadWrite, true);
    this->Level4Address->entries[indexer.PDP_i]=PDE;
  } else {
    PDP=(PageTable*)((uint64_t)PDE.GetAddress()<<12);
  }
  PDE=PDP->entries[indexer.PD_i];
  PageTable* PD;
  if (!PDE.GetFlag(PT_Flag::Present)) {
    PD=(PageTable*)Allocator.RequestPage();
    memset(PD, 0, 0x1000);
    PDE.SetAddress((uint64_t)PD>>12);
    PDE.SetFlag(PT_Flag::Present, true);
    PDE.SetFlag(PT_Flag::ReadWrite, true);
    PDP->entries[indexer.PD_i]=PDE;
  } else {
    PD=(PageTable*)((uint64_t)PDE.GetAddress()<<12);
  }
  PDE=PD->entries[indexer.PT_i];
  PageTable* PT;
  if (!PDE.GetFlag(PT_Flag::Present)) {
    PT=(PageTable*)Allocator.RequestPage();
    memset(PT, 0, 0x1000);
    PDE.SetAddress((uint64_t)PT>>12);
    PDE.SetFlag(PT_Flag::Present, true);
    PDE.SetFlag(PT_Flag::ReadWrite, true);
    PD->entries[indexer.PT_i]=PDE;
  } else {
    PT=(PageTable*)((uint64_t)PDE.GetAddress()<<12);
  }
  PDE=PT->entries[indexer.P_i];
  PDE.SetAddress((uint64_t)physicalMemory>>12);
  PDE.SetFlag(PT_Flag::Present, true);
  PDE.SetFlag(PT_Flag::ReadWrite, true);
  PT->entries[indexer.P_i] = PDE;
}

void* heapStart;
void* heapEnd;
HeapSegHdr* LastHdr;

void InitializeHeap(void* heapAddress, size_t pageCount) {
  void* pos = heapAddress;
  for (size_t i = 0; i < pageCount; i++){
    pageTableManager.MapMemory(pos, Allocator.RequestPage());
    pos = (void*)((size_t)pos + 0x1000);
  }
  size_t heapLength = pageCount * 0x1000;
  heapStart = heapAddress;
  heapEnd = (void*)((size_t)heapStart + heapLength);
  HeapSegHdr* startSeg = (HeapSegHdr*)heapAddress;
  startSeg->length = heapLength - sizeof(HeapSegHdr);
  startSeg->next = NULL;
  startSeg->last = NULL;
  startSeg->free = true;
  LastHdr = startSeg;
}

void free(void* address) {
  HeapSegHdr* segment = (HeapSegHdr*)address - 1;
  segment->free = true;
  segment->CombineForward();
  segment->CombineBackward();
}

void* malloc(size_t size) {
  if (size % 0x10 > 0) {
    size -= (size % 0x10);
    size += 0x10;
  }
  if (size == 0) return NULL;
  HeapSegHdr* currentSeg = (HeapSegHdr*) heapStart;
  while(true) {
    if (currentSeg->free) {
      if (currentSeg->length > size) {
        currentSeg->Split(size);
        currentSeg->free = false;
        return (void*)((uint64_t)currentSeg + sizeof(HeapSegHdr));
      }
      if (currentSeg->length == size) {
        currentSeg->free = false;
        return (void*)((uint64_t)currentSeg + sizeof(HeapSegHdr));
      }
    }
    if (currentSeg->next == NULL) break;
    currentSeg = currentSeg->next;
  }
  ExpandHeap(size);
  return malloc(size);
}

HeapSegHdr* HeapSegHdr::Split(size_t splitLength) {
  if (splitLength < 0x10) return NULL;
  int64_t splitSegLength = length - splitLength - (sizeof(HeapSegHdr));
  if (splitSegLength < 0x10) return NULL;
  HeapSegHdr* newSplitHdr = (HeapSegHdr*) ((size_t)this + splitLength + sizeof(HeapSegHdr));
  next->last = newSplitHdr;
  newSplitHdr->next = next;
  next = newSplitHdr;
  newSplitHdr->last = this;
  newSplitHdr->length = splitSegLength;
  newSplitHdr->free = free; 
  length = splitLength;

  if (LastHdr == this) LastHdr = newSplitHdr;
  return newSplitHdr;
}

void ExpandHeap(size_t length) {
  if (length % 0x1000) {
    length -= length % 0x1000;
    length += 0x1000;
  }
  size_t pageCount = length / 0x1000;
  HeapSegHdr* newSegment = (HeapSegHdr*)heapEnd;
  for (size_t i = 0; i < pageCount; i++) {
    pageTableManager.MapMemory(heapEnd, Allocator.RequestPage());
    heapEnd = (void*)((size_t)heapEnd + 0x1000);
  }
  newSegment->free = true;
  newSegment->last = LastHdr;
  LastHdr->next = newSegment;
  LastHdr = newSegment;
  newSegment->next = NULL;
  newSegment->length = length - sizeof(HeapSegHdr);
  newSegment->CombineBackward();
}

void HeapSegHdr::CombineForward() {
  if (next == NULL) return;
  if (!next->free) return;
  if (next == LastHdr) LastHdr = this;
  if (next->next != NULL) next->next->last = this;
  length = length + next->length + sizeof(HeapSegHdr);
}

void HeapSegHdr::CombineBackward() {
  if (last != NULL && last->free) last->CombineForward();
}

void memcpy(void *dest, void *src, size_t n) {
  int i;
  char* src_char = (char*)src;
  char* dest_char = (char*)dest;
  for (i=0; i<n; i++) dest_char[i] = src_char[i];
}