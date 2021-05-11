/*
 * Copyright Null LLC
 * Please read the License!
 *  _     _           _ 
 * | |   | |         | |
 * | |__ | |_   _  _ | | ____ ____
 * |  __)| | | | |/ || |/ ___) _  |     bitmap.hpp
 * | |   | | |_| ( (_| | |  ( ( | |     Array of bits.
 * |_|   |_|\__  |\____|_|   \_||_|
 *         (____/
 */

#pragma once
#include <stddef.h>
#include <stdint.h>

class Bitmap{
    public:
        size_t Size;
        uint8_t* Buffer;
        bool operator[](uint64_t index);
        bool Set(uint64_t index, bool value);
};