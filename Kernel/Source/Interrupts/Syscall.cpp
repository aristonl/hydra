//========= Copyright N11 Software, All rights reserved. ============//
//
// File: Syscall.cpp
// Purpose: Syscall Handler
// Maintainer: FiReLScar
//
//===================================================================//

#include <Interrupts/Syscall.hpp>
#include <COM.hpp>

void SyscallHandler(void*) {
    kprintf("Hello World!\n\r");
}
