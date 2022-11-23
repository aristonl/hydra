#include <Memory/Memory.hpp>
#include <Memory/Mem_.hpp>

long long FindFirstFreeBit(long long count, Bitmap* map) {
    for (unsigned long long i=0;i<count/64;i++) {
        if (map->map[i] == 0xFFFFFFFFFFFFFFFF) {
            for (int x=0;x<64;x++) {
                int bit = 1<<x;
                if (!(map->map[i]&bit)) return i*64+x;
            }
        }
    }
    return -1;
}

long long Memory::GetBlockCount() {
    return this->size/64;
}

void Memory::Allocate(unsigned long long base, unsigned long long size) {
    long long align = base/0x1000;
    long long blocks = size/0x1000;
    for (;blocks>0;blocks--) {
        map.ClearBit(align++);
        usedBlocks--;
    }
    map.SetBit(0);
}

void Memory::Free(unsigned long long base, unsigned long long size) {
    long long align = base/0x1000;
    long long blocks = size/0x1000;
    for (;blocks>0;blocks--) {
        map.SetBit(align++);
        usedBlocks++;
    }
}

void* Memory::Allocate() {
    if (maxBlocks-usedBlocks <= 0) return 0;
    long long frame = FindFirstFreeBit(GetBlockCount(), &map);
    if (frame == -1) return 0;
    map.SetBit(frame);
    unsigned long long address = frame*0x1000;
    usedBlocks++;
    return (void*)address;
}

void Memory::Free(void* base) {
    unsigned long long address = (unsigned long long)base;
    long long frame = address / 0x1000;
    map.ClearBit(frame);
    usedBlocks--;
}

Memory::Memory(BOB* bob) {
    this->size = bob->MapSize;
    this->maxBlocks = this->size*1024;
    this->maxBlocks = GetBlockCount()*1024;
    this->usedBlocks = GetBlockCount();

    memset(map.map, 0xf, GetBlockCount()/8);
}

void Bitmap::SetBit(long long bit) {
    this->map[bit/64] |= (1<<(bit%64));
}

void Bitmap::ClearBit(long long bit) {
    this->map[bit/64] &= ~(1<<(bit%64));
}

bool Bitmap::GetBit(long long bit) {
    return this->map[bit/64] & (1<<(bit%64));
}