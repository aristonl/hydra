//========= Copyright N11 Software, All rights reserved. ============//
//
// File: PageFault.cpp
// Purpose: 
// Maintainer: FiReLScar
//
//===================================================================//

#include <Interrupts/PageFault.hpp>
#include <COM.hpp>

void PageFault(void*) {
    kprintf("\r\e[31m[ERROR] Page Fault\e[0m\n\r");
    asm("hlt");
}
