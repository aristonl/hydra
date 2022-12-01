//========= Copyright N11 Software, All rights reserved. ============//
//
// File: cpuid.cpp
// Maintainer: aristonl
//
//===================================================================//

/* 
 * This file will probably have to move to 'arch/x86/' at some point
 * but considering that we don't have code for any other architecture,
 * I don't plan on moving it.
 */ 

#include <cpu/cpuid.h>

#include <COM.hpp>
#include <stddef.h>

int detect_cpu(void) {
    unsigned long ebx, unused;
    cpuid(0, unused, ebx, unused, unused);
    switch (ebx) {
    case 0x756e6547: /* Intel Magic Code */
        kprintf("\r\e[92m[INFO] Intel CPU Detected\n");
        break;
    case 0x68747541: /* AMD Magic Code */
        kprintf("\r\e[92m[INFO] AMD CPU Detected\n");
        break;
    default:
        kprintf("\r\e[31m[ERROR] Unknown x86 CPU Detected\n");
        break;
    }
    return 0;
}

int get_model(void) {
    unsigned long ebx, unused;
    cpuid(0, unused, ebx, unused, unused);
    return ebx;
}
