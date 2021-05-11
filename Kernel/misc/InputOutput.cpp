/*
 * Copyright Null LLC
 * Please read the License!
 *  _     _           _ 
 * | |   | |         | |
 * | |__ | |_   _  _ | | ____ ____
 * |  __)| | | | |/ || |/ ___) _  |     InputOutput.cpp
 * | |   | | |_| ( (_| | |  ( ( | |     Controls port data.
 * |_|   |_|\__  |\____|_|   \_||_|
 *         (____/
 */

#include "InputOutput.hpp"

void outb(uint16_t port, uint8_t value) {
    asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

uint8_t inb(uint16_t port) {
    uint8_t returnVal;
    asm volatile ("inb %1, %0" : "=a"(returnVal) : "Nd"(port));
    return returnVal;
}

void io_wait() {asm volatile ("outb %%al, $0x80" : : "a"(0));}