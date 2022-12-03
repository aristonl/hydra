//========= Copyright N11 Software, All rights reserved. ============//
//
// File: CPUID.hpp
// Maintainer: aristonl, levih
//
//===================================================================//

#pragma once

/*
 * Removed the detect and brand, as they won't be used for a while
 * and they didn't return the value, instead printed it,
 * which will be hard to work with. I attempted to put it in string form,
 * the compiler didn't appreciate that. So for now it will be removed.
 * 
 * Also we won't need to move the code to a separate directory for the 
 * architecture because we will just use macros.
 * 
 * - Levi
 */

#define cpuid(in, a, b, c, d) __asm__ __volatile__ ("cpuid" : "=a"(a), "=b"(b), "=c"(c), "=d"(d) : "0"(in));