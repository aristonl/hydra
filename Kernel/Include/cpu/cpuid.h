//========= Copyright N11 Software, All rights reserved. ============//
//
// File: cpuid.h
// Maintainer: aristonl
//
//===================================================================//

#pragma once

#include <COM.hpp>
#include <stddef.h>

#define cpuid(in, a, b, c, d) __asm__("cpuid"                              \
                                      : "=a"(a), "=b"(b), "=c"(c), "=d"(d) \
                                      : "a"(in));

int detect_cpu(void);
int get_model(void);
