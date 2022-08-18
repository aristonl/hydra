#ifndef COM
#define COM
#include <IO.hpp>

void InitializeSerialDevice();
char AwaitSerialResponse();
void kputchar(char a);
int kprintf(const char* fmt, ...);

#endif
