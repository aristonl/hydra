/*
 * Copyright Null LLC
 * Please read the License!
 *  _     _           _ 
 * | |   | |         | |
 * | |__ | |_   _  _ | | ____ ____
 * |  __)| | | | |/ || |/ ___) _  |     memory.hpp
 * | |   | | |_| ( (_| | |  ( ( | |     Controls memory/ram usage and permissions.
 * |_|   |_|\__  |\____|_|   \_||_|
 *         (____/
 */

#pragma once
#include <stdint.h>
#include "../misc/bootloader.hpp"
#include "bitmap.hpp"

extern const char* EFI_MEMORY_TYPE_STRINGS[];
uint64_t getMemorySize(EFI_MEMORY_DESCRIPTOR* Map, uint64_t MapEntries, uint64_t MapDescriptorSize);
void memset(void* start, uint8_t value, uint64_t num);
uint64_t getFreeMemory();
uint64_t getReservedMemory();
uint64_t getUsedMemory();
uint64_t getTotalMemory();

class PageFrameAllocator {
    public:
        void ReadEFIMemoryMap(EFI_MEMORY_DESCRIPTOR* mMap, size_t mMapSize, size_t mMapDescSize);
        Bitmap PageBitmap;
        void FreePage(void* address);
        void FreePages(void* address, uint64_t pageCount);
        void LockPage(void* address);
        void LockPages(void* address, uint64_t pageCount);
        void* RequestPage();
        uint64_t GetFreeRAM();
        uint64_t GetUsedRAM();
        uint64_t GetReservedRAM();
    private:
        void InitBitmap(size_t bitmapSize, void* bufferAddress);
        void ReservePage(void* address);
        void ReservePages(void* address, uint64_t pageCount);
        void UnreservePage(void* address);
        void UnreservePages(void* address, uint64_t pageCount);
};

extern PageFrameAllocator Allocator;

enum PT_Flag {
    Present = 0,
    ReadWrite = 1,
    UserSuper = 2,
    WriteThrough = 3,
    CacheDisabled = 4,
    Accessed = 5,
    LargerPages = 7,
    Custom0 = 9,
    Custom1 = 10,
    Custom2 = 11,
    NX = 63
};

struct PageDirectoryEntry {
    uint64_t Value;
    void SetFlag(PT_Flag flag, bool enabled);
    bool GetFlag(PT_Flag flag);
    void SetAddress(uint64_t address);
    uint64_t GetAddress();
};

struct PageTable { 
    PageDirectoryEntry entries [512];
}__attribute__((aligned(0x1000)));

class PageMapIndexer {
    public:
        PageMapIndexer(uint64_t virtualAddress);
        uint64_t PDP_i;
        uint64_t PD_i;
        uint64_t PT_i;
        uint64_t P_i;
};

class PageTableManager {
    public:
        PageTableManager(PageTable* PML4Address);
        PageTable* PML4;
        void MapMemory(void* virtualMemory, void* physicalMemory);
};