#pragma once

#include "sys/cdefs.h"
#include "sys/types.h"

__BEGIN_DECLS

int strlen(const char* str);
int strcmp(const char*, const char*);
char* strcpy(char* dest, const char* src);
int memcmp(const void*, const void*, size_t);
void *memcpy(void *, const void *, size_t);
void* memset(void* str, int c, size_t n);

const char* strerror(int errnum);

char *strcat(char *, const char *);

__END_DECLS
