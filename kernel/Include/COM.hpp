//========= Copyright N11 Software, All rights reserved. ============//
//
// File: COM.hpp
// Purpose:
// Maintainer: FiReLScar
//
//===================================================================//

#pragma once
#include <sys/io.h>
#include <Framebuffer.hpp>

void InitializeSerialDevice();
char AwaitSerialResponse();
void kputchar(char a);
int kprintf(const char* fmt, ...);
void initFB(Framebuffer* fb);