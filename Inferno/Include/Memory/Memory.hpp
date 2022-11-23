//========= Copyright N11 Software, All rights reserved. ============//
//
// File: Memory.hpp
// Purpose:
// Maintainer: FiReLScar
//
//===================================================================//

#pragma once
#include <BOB.hpp>

class Bitmap {
    public:
        void SetBit(long long bit);
        void ClearBit(long long bit);
        bool GetBit(long long bit);
        unsigned long long* map;
};

class Memory {
    public:
        Memory(BOB* bob);
        long long GetBlockCount();
        void Allocate(unsigned long long base, unsigned long long size);
        void Free(unsigned long long base, unsigned long long size);
        void* Allocate(), Free(void* base);
    private:
        Bitmap map;
        unsigned long long size, usedBlocks, maxBlocks;
};

struct memory_region {
	unsigned long long startLo;
	unsigned long long startHi;
	unsigned long long sizeLo;
	unsigned long long sizeHi;
	unsigned long long type;
	unsigned long long acpi_3_0;
};