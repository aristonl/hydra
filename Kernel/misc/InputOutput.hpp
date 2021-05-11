/*
 * Copyright Null LLC
 * Please read the License!
 *  _     _           _ 
 * | |   | |         | |
 * | |__ | |_   _  _ | | ____ ____
 * |  __)| | | | |/ || |/ ___) _  |     InputOutput.hpp
 * | |   | | |_| ( (_| | |  ( ( | |     Controls port data.
 * |_|   |_|\__  |\____|_|   \_||_|
 *         (____/
 */

#pragma once
#include <stdint.h>

void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);
void io_wait();