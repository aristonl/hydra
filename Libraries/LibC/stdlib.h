#pragma once

#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

void* malloc(long unsigned int);
void free(void*);
void* calloc(long unsigned int nmemb, long unsigned int);
void* realloc(void *ptr, long unsigned int);

void exit(int status);
void abort();

__END_DECLS
