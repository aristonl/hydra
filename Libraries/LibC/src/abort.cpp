#include <stdio.h>
#include <stdlib.h>

__attribute__((__notreturn__))
void abort(void) {
#if defined(__is_libk) // for inferno usage
	// setup kernel abort
	asm volatile("hlt");
#else
	printf("abort()\n");
#endif
	while (1) {}
	__builtin_unreachable();	
}
