#include <Memory/Memory.hpp>

unsigned long long Size = 0;
BOB* bob;

void Memory::Init(BOB* bob) {
    for (unsigned long long i=0;i<bob->MapSize/bob->DescriptorSize; i++) {
        MemoryDescriptor* descriptor = (MemoryDescriptor*)((unsigned long long)bob->MemoryMap + (i * bob->DescriptorSize));
        Size+=descriptor->NumberOfPages*4096;
    }
}

unsigned long long Memory::GetSize() {
    return Size;
}