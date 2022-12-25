//========= Copyright N11 Software, All rights reserved. ============//
//
// File: COM.hpp
// Purpose:
// Maintainer: FiReLScar
//
//===================================================================//

#pragma once
#include <sys/io.h>

void InitializeSerialDevice();
char AwaitSerialResponse();
void kputchar(char a);
int kprintf(const char* fmt, ...);
