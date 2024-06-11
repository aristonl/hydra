//========= Copyright N11 Software, All rights reserved. ============//
//
// File: COM.h
// Purpose:
// Maintainer: FiReLScar
//
//===================================================================//

#pragma once
#include <sys/io.h>
#include <Drivers/TTY/Framebuffer.h>

void InitializeSerialDevice();
char AwaitSerialResponse();
void kputchar(char a);
int kprintf(const char* fmt, ...);
void initFB(Framebuffer* fb);
