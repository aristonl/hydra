//========= Copyright N11 Software, All rights reserved. ============//
//
// File: assert.h
// Purpose: Header file for POSIX program assertion
// Author: Ariston Lorenzo <aristonl@n11.dev>
//
//===================================================================//

#pragma once

#include <sys/cdefs.h>

#ifdef NDEBUG
#    define assert(e) ((void)0)
#endif /* NDEBUG */

__BEGIN_DECLS

void __assert_fail(const char* msg, const char* file, unsigned line, const char* function);

// This isn't used but it's for standard compliance
void __assert(const char* msg, const char* file, unsigned line);

#define assert(e) (static_cast<bool>(e) ? void(0) : __assert(#e, __FILE__, __LINE__, __PRETTY_FUNCTION__))
#define ASSERT assert
#define ASSERT_NOT_REACHED assert(false)

__END_DECLS
