//========= Copyright N11 Software, All rights reserved. ============//
//
// File: assert.cpp
// Purpose: file for POSIX program assertion
// Author: Ariston Lorenzo <aristonl@n11.dev>
//
//===================================================================//

#include "assert.h"
#include "stdlib.h"
#include "stdio.h"

extern "C" {

	extern void __assert_fail(const char* msg, const char* file, unsigned line, const char* function) {
		printf("ASSERTION FAILED: %s\n:%s in %d\n", msg, file, line, function);
		abort();
	}

}
