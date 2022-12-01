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

static __inline unsigned int
get_cpuid_max (unsigned int __ext, unsigned int *__sig) {
  unsigned int __eax, __ebx, __ecx, __edx;


  /* Host supports cpuid.  Return highest supported cpuid input value.  */
  cpuid (__ext, __eax, __ebx, __ecx, __edx);

  if (__sig)
    *__sig = __ebx;

  return __eax;
}

static __inline int
get_cpuid (unsigned int __leaf,
	     unsigned int *__eax, unsigned int *__ebx,
	     unsigned int *__ecx, unsigned int *__edx) {
  unsigned int __ext = __leaf & 0x80000000;
  unsigned int __maxlevel = get_cpuid_max (__ext, 0);

  if (__maxlevel == 0 || __maxlevel < __leaf)
    return 0;

  cpuid (__leaf, *__eax, *__ebx, *__ecx, *__edx);
  return 1;
}

int detect_cpu(void) {
    unsigned long ebx, unused;
    cpuid(0, unused, ebx, unused, unused);
    switch (ebx) {
    case 0x756e6547: /* Intel Magic Code */
        kprintf("\r\e[92m[KERNEL] cpu: Intel CPU Detected\n");
        break;
    case 0x68747541: /* AMD Magic Code */
        kprintf("\r\e[92m[KERNEL] cpu: AMD CPU Detected\n");
        break;
    default:
        kprintf("\r\e[91m[KERNEL] Kernel warning - Unknown x86 CPU Detected\n");
        break;
    }
    return 0;
}

int cpu_brand(void) {
    unsigned int brand[12];

    get_cpuid(0x80000002, brand+0x0, brand+0x1, brand+0x2, brand+0x3);
    get_cpuid(0x80000003, brand+0x4, brand+0x5, brand+0x6, brand+0x7);
    get_cpuid(0x80000004, brand+0x8, brand+0x9, brand+0xa, brand+0xb);
    kprintf("\r\e[92m[KERNEL] cpu: CPU0: %s\n", brand);
    return 0;
}