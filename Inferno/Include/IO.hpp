//========= Copyright N11 Software, All rights reserved. ============//
//
// File: IO.hpp
// Purpose:
// Maintainer: FiReLScar
//
//===================================================================//

#pragma once

void outb(unsigned short port, unsigned char value);
unsigned char inb(unsigned short port);
void io_wait();