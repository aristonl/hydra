#pragma once

#include "sys/cdefs.h"
#include "sys/types.h"

__BEGIN_DECLS

size_t strlen(const char*);
int strcmp(const char*, const char*);
int memcmp(const void*, const void*, size_t);
void *memcpy(void *, const void *, size_t);
void* memset(void* str, int c, size_t n);

const char* strerror(int errnum);

char *strcat(char *, const char *);

__END_DECLS
