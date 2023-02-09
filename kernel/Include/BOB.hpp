//========= Copyright N11 Software, All rights reserved. ============//
//
// File: BOB.hpp
// Purpose:
// Maintainer: FiReLScar
//
//===================================================================//

#pragma once
#include <Framebuffer.hpp>

struct MemoryDescriptor {
    unsigned int Type;
    unsigned long long PhysicalStart, VirtualStart, NumberOfPages, Attribute;
};

struct BOB {
    unsigned long long DescriptorSize, MapSize;
    MemoryDescriptor* MemoryMap;
    void* RSDP;
    Framebuffer* framebuffer;
};