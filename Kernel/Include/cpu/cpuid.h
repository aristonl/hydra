//========= Copyright N11 Software, All rights reserved. ============//
//
// File: cpuid.h
// Maintainer: aristonl
//
//===================================================================//

#pragma once

#include <COM.hpp>
#include <stddef.h>

#define cpuid(in, a, b, c, d) __asm__ __volatile__ ("cpuid\n\t"                              \
                                      : "=a"(a), "=b"(b), "=c"(c), "=d"(d) \
                                      : "0"(in));

static inline unsigned int
get_cpuid_max (unsigned int __ext, unsigned int *__sig);

static inline int
get_cpuid (unsigned int __leaf,
	     unsigned int *__eax, unsigned int *__ebx,
	     unsigned int *__ecx, unsigned int *__edx);

static inline void native_cpuid(unsigned int *eax, unsigned int *ebx,
                                unsigned int *ecx, unsigned int *edx) {
    /* ecx is often an input as well as an output. */
        asm volatile("cpuid"
            : "=a" (*eax),
              "=b" (*ebx),
              "=c" (*ecx),
              "=d" (*edx)
            : "0" (*eax), "2" (*ecx));
}
                            

int detect_cpu(void);
int cpu_brand(void);
