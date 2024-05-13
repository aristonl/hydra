//========= Copyright N11 Software, All rights reserved. ============//
//
// File: DoublePageFault.cpp
// Purpose:
// Maintainer: FiReLScar
//
//===================================================================//

#include <COM.hpp>
#include <Interrupts/DoublePageFault.hpp>

void DoublePageFault(void*) {
    kprintf("\r\e[31m[ERROR] Double Page Fault\e[0m\n\r");
    asm("hlt");
}
