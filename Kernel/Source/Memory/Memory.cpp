#include <Memory/Memory.hpp>
#include <stddef.h>
#include <Memory/Paging.h>
#include <Memory/Mem_.hpp>
#include <Log.h>
#include <Drivers/RTC/RTC.h>

unsigned long long Size = 0;
BOB* bob;
unsigned long long free = 0, used = 0;

void Memory::Init(BOB* bob) {
    void* bitmapAddress = (void*)0;
    size_t bitmapSize = 0;
    for (unsigned long long i=0;i<bob->MapSize/bob->DescriptorSize; i++) {
        MemoryDescriptor* descriptor = (MemoryDescriptor*)((unsigned long long)bob->MemoryMap + (i * bob->DescriptorSize));
        Size+=descriptor->NumberOfPages*4096;
        if (descriptor->Type == 7 && descriptor->NumberOfPages > bitmapSize) {
            bitmapAddress = (void*)descriptor->PhysicalStart;
            bitmapSize = descriptor->NumberOfPages;
        }
    }

    bitmapSize*=4096;

    free = Size;

    // Enable 4K allocation aka paging
    memset(bitmapAddress, 0, bitmapSize);
    Paging::map.Buffer = (unsigned char*)bitmapAddress;
    Paging::map.Size = bitmapSize;

    Paging::AllocatePages(Paging::map.Buffer, Paging::map.Size/4096+1);

    for (unsigned long long i=0;i<bob->MapSize/bob->DescriptorSize; i++) {
        MemoryDescriptor* descriptor = (MemoryDescriptor*)((unsigned long long)bob->MemoryMap + (i * bob->DescriptorSize));
        if (descriptor->Type != 7)
            Paging::AllocatePages((void*)descriptor->PhysicalStart, descriptor->NumberOfPages);
    }
}

unsigned long long Memory::GetSize() { return Size; }
unsigned long long Memory::GetFree() { return free; }
unsigned long long Memory::GetUsed() { return used; }

void Memory::SetFree(unsigned long long free) { free = free; }
void Memory::SetUsed(unsigned long long used) { used = used; }