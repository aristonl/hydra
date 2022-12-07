//========= Copyright N11 Software, All rights reserved. ============//
//
// File: brk.cpp
// Purpose: Implementations of brk() and sbrk()
// Maintainer: aristonl
//
//===================================================================//

#include <unistd.h>
#include <stdint.h>
#include <errno.h>

void *sbrk(intptr_t increment) {
	extern char __heap_start;
	extern char __heap_end;

	static char *heap_end;
	char *prev_heap_end;

	if (0 == heap_end)
		heap_end = &__heap_start;

	prev_heap_end = heap_end;
	heap_end += increment;

	if (heap_end < (&__heap_end)) {

	} else {
		errno = ENOMEM;
		return (char*)-1;
	}

	return (void *) prev_heap_end;
}
