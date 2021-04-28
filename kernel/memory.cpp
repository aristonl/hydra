#include "memory.h"

uint64_t freeMemory;
uint64_t reservedMemory;
uint64_t usedMemory;
bool Initialized = false;
PageFrameAllocator Allocator;

const char* EFI_MEMORY_TYPE_STRINGS[] {
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
};

uint64_t getMemorySize(EFI_MEMORY_DESCRIPTOR* Map, uint64_t MapEntries, uint64_t MapDescriptorSize) {
    static uint64_t memorySizeBytes = 0;
    if (memorySizeBytes > 0) return memorySizeBytes;
    for (int i = 0; i < MapEntries; i++) {
        EFI_MEMORY_DESCRIPTOR* descriptor = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)Map + (i * MapDescriptorSize));
        memorySizeBytes += descriptor->numPages * 4096;
    }
    return memorySizeBytes;
}

void memset(void* start, uint8_t value, uint64_t num){
    for (uint64_t i = 0; i < num; i++) *(uint8_t*)((uint64_t)start + i) = value;
}

uint64_t getFreeMemory() {
    return freeMemory;
}

uint64_t getReservedMemory() {
    return reservedMemory;
}

uint64_t getUsedMemory() {
    return usedMemory;
}

uint64_t getTotalMemory() {
    return freeMemory + reservedMemory + usedMemory;
}

void PageFrameAllocator::ReadEFIMemoryMap(EFI_MEMORY_DESCRIPTOR* mMap, size_t mMapSize, size_t mMapDescSize){
    if (Initialized) return;
    Initialized = true;
    uint64_t mMapEntries = mMapSize / mMapDescSize;
    void* largestFreeMemSeg = NULL;
    size_t largestFreeMemSegSize = 0;
    for (int i = 0; i < mMapEntries; i++) {
        EFI_MEMORY_DESCRIPTOR* descriptor = (EFI_MEMORY_DESCRIPTOR*)((uint64_t) mMap + (i * mMapDescSize));
        if (descriptor->type == 7) {
            if (descriptor->numPages * 4096 > largestFreeMemSegSize) {
                largestFreeMemSeg = descriptor->physAddr;
                largestFreeMemSegSize = descriptor->numPages * 4096;
            }
        }
    }
    uint64_t memorySize = getMemorySize(mMap, mMapEntries, mMapDescSize);
    freeMemory = memorySize;
    uint64_t bitmapSize = memorySize / 4096 / 8 + 1;
    InitBitmap(bitmapSize, largestFreeMemSeg);
    LockPages(&PageBitmap, PageBitmap.Size / 4096 + 1);
    for (int i = 0; i < mMapEntries; i++) {
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t) mMap + (i * mMapDescSize));
        if (desc->type != 7) ReservePages(desc->physAddr, desc->numPages);
    }
}

void PageFrameAllocator::InitBitmap(size_t bitmapSize, void* bufferAddress){
    PageBitmap.Size = bitmapSize;
    PageBitmap.Buffer = (uint8_t*) bufferAddress;
    for (int i = 0; i < bitmapSize; i++) *(uint8_t*)(PageBitmap.Buffer + i) = 0;
}

uint64_t pageBitmapIndex = 0;
void* PageFrameAllocator::RequestPage(){
    for (; pageBitmapIndex < PageBitmap.Size * 8; pageBitmapIndex++) {
        if (PageBitmap[pageBitmapIndex] == true) continue;
        LockPage((void*)(pageBitmapIndex * 4096));
        return (void*)(pageBitmapIndex * 4096);
    }
    return NULL;
}

void PageFrameAllocator::FreePage(void* address){
    uint64_t index = (uint64_t) address / 4096;
    if (PageBitmap[index] == false) return;
    if (PageBitmap.Set(index, false)) {
        freeMemory += 4096;
        usedMemory -= 4096;
        if (pageBitmapIndex > index) pageBitmapIndex = index;
    }
}

void PageFrameAllocator::FreePages(void* address, uint64_t pageCount){
    for (int t = 0; t < pageCount; t++) FreePage((void*)((uint64_t) address + (t * 4096)));
}

void PageFrameAllocator::LockPage(void* address){
    uint64_t index = (uint64_t) address / 4096;
    if (PageBitmap[index] == true) return;
    if (PageBitmap.Set(index, true)) {
        freeMemory -= 4096;
        usedMemory += 4096;
    }
}

void PageFrameAllocator::LockPages(void* address, uint64_t pageCount){
    for (int t = 0; t < pageCount; t++) LockPage((void*)((uint64_t) address + (t * 4096)));
}

void PageFrameAllocator::UnreservePage(void* address){
    uint64_t index = (uint64_t)address / 4096;
    if (PageBitmap[index] == false) return;
    if (PageBitmap.Set(index, false)) {
        freeMemory += 4096;
        reservedMemory -= 4096;
    }
}

void PageFrameAllocator::UnreservePages(void* address, uint64_t pageCount){
    for (int t = 0; t < pageCount; t++) UnreservePage((void*)((uint64_t) address + (t * 4096)));
}

void PageFrameAllocator::ReservePage(void* address){
    uint64_t index = (uint64_t) address / 4096;
    if (PageBitmap[index] == true) return;
    if (PageBitmap.Set(index, true)) {
        freeMemory -= 4096;
        reservedMemory += 4096;
        if (pageBitmapIndex > index) pageBitmapIndex = index;
    }
}

void PageFrameAllocator::ReservePages(void* address, uint64_t pageCount){
    for (int t = 0; t < pageCount; t++) ReservePage((void*)((uint64_t)address + (t * 4096)));
}

uint64_t PageFrameAllocator::GetFreeRAM() {return freeMemory;}
uint64_t PageFrameAllocator::GetUsedRAM() {return usedMemory;}
uint64_t PageFrameAllocator::GetReservedRAM() {return reservedMemory;}

PageMapIndexer::PageMapIndexer(uint64_t virtualAddress) {
    virtualAddress >>= 12;
    P_i = virtualAddress & 0x1ff;
    virtualAddress >>= 9;
    PT_i = virtualAddress & 0x1ff;
    virtualAddress >>= 9;
    PD_i = virtualAddress & 0x1ff;
    virtualAddress >>= 9;
    PDP_i = virtualAddress & 0x1ff;
}

PageTableManager::PageTableManager(PageTable* PML4Address) {this->PML4 = PML4Address;}

void PageTableManager::MapMemory(void* virtualMemory, void* physicalMemory) {
    PageMapIndexer indexer = PageMapIndexer((uint64_t) virtualMemory);
    PageDirectoryEntry PDE;
    PDE = PML4->entries[indexer.PDP_i];
    PageTable* PDP;
    if (!PDE.GetFlag(Present)) {
        PDP = (PageTable*) Allocator.RequestPage();
        memset(PDP, 0, 0x1000);
        PDE.SetAddress((uint64_t) PDP >> 12);
        PDE.SetFlag(Present, true);
        PDE.SetFlag(ReadWrite, true);
        PML4->entries[indexer.PDP_i] = PDE;
    } else PDP = (PageTable*)((uint64_t) PDE.GetAddress() << 12);
    PDE = PDP->entries[indexer.PD_i];
    PageTable* PD;
    if (!PDE.GetFlag(Present)) {
        PD = (PageTable*) Allocator.RequestPage();
        memset(PD, 0, 0x1000);
        PDE.SetAddress((uint64_t) PD >> 12);
        PDE.SetFlag(Present, true);
        PDE.SetFlag(ReadWrite, true);
        PDP->entries[indexer.PD_i] = PDE;
    } else PD = (PageTable*)((uint64_t) PDE.GetAddress() << 12);
    PDE = PD->entries[indexer.PT_i];
    PageTable* PT;
    if (!PDE.GetFlag(Present)) {
        PT = (PageTable*) Allocator.RequestPage();
        memset(PT, 0, 0x1000);
        PDE.SetAddress((uint64_t) PT >> 12);
        PDE.SetFlag(Present, true);
        PDE.SetFlag(ReadWrite, true);
        PD->entries[indexer.PT_i] = PDE;
    } else PT = (PageTable*)((uint64_t) PDE.GetAddress() << 12);
    PDE = PT->entries[indexer.P_i];
    PDE.SetAddress((uint64_t) physicalMemory >> 12);
    PDE.SetFlag(Present, true);
    PDE.SetFlag(ReadWrite, true);
    PT->entries[indexer.P_i] = PDE;
}

void PageDirectoryEntry::SetFlag(PT_Flag flag, bool enabled){
    uint64_t bitSelector = (uint64_t) 1 << flag;
    Value &= ~bitSelector;
    if (enabled) Value |= bitSelector;
}

bool PageDirectoryEntry::GetFlag(PT_Flag flag){
    uint64_t bitSelector = (uint64_t) 1 << flag;
    return Value & bitSelector > 0 ? true : false;
}

uint64_t PageDirectoryEntry::GetAddress(){
    return (Value & 0x000ffffffffff000) >> 12;
}

void PageDirectoryEntry::SetAddress(uint64_t address){
    address &= 0x000000ffffffffff;
    Value &= 0xfff0000000000fff;
    Value |= (address << 12);
}