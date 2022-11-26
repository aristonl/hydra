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
    register char* arg0 asm("rax");
    register char* arg1 asm("rbx");
    register char* arg2 asm("rcx");
    register char* arg3 asm("rdx");
    kprintf("%s\n\r", arg3);
}
