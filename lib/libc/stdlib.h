#pragma once

#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

void* malloc(size_t size);
void free(void* ptr);
void* calloc(long unsigned int nmemb, long unsigned int);
void* realloc(void* ptr, long unsigned int);

void exit(int status);
void abort();

int abs(int n);

__END_DECLS
