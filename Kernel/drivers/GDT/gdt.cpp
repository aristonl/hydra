/*
 * Copyright Null LLC
 * Please read the License!
 *  _     _           _ 
 * | |   | |         | |
 * | |__ | |_   _  _ | | ____ ____
 * |  __)| | | | |/ || |/ ___) _  |     gdt.cpp
 * | |   | | |_| ( (_| | |  ( ( | |     Info about the CPU
 * |_|   |_|\__  |\____|_|   \_||_|
 *         (____/
 */

#include "gdt.hpp"

__attribute__ ((aligned(0x1000)))
GDT DefaultGDT = {
    {0, 0, 0, 0x00, 0x00, 0},
    {0, 0, 0, 0x9a, 0xa0, 0},
    {0, 0, 0, 0x92, 0xa0, 0},
    {0, 0, 0, 0x00, 0x00, 0},
    {0, 0, 0, 0x9a, 0xa0, 0},
    {0, 0, 0, 0x92, 0xa0, 0},
};