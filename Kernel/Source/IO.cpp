//========= Copyright N11 Software, All rights reserved. ============//
//
// File: IO.cpp
// Purpose:
// Maintainer: FiReLScar
//
//===================================================================//

#include <IO.hpp>

void outb(unsigned short port, unsigned char value) {
    asm volatile("outb %0, %1"
                 :
                 : "a"(value), "Nd"(port));
}

unsigned char inb(unsigned short port) {
    unsigned char returnVal;
    asm volatile("inb %1, %0"
                 : "=a"(returnVal)
                 : "Nd"(port));
    return returnVal;
}

void io_wait() {
    asm volatile("outb %%al, $0x80" ::"a"(0));
}
