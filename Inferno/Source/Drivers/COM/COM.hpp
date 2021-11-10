#pragma once
#include "../IO/IO.hpp"

int InitializeSerialDevice();
char AwaitSerialResponse();
void kputchar(char a);
void kprintf(const char* str);