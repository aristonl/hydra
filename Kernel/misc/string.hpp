/*
 * Copyright Null LLC
 * Please read the License!
 *  _     _           _ 
 * | |   | |         | |
 * | |__ | |_   _  _ | | ____ ____
 * |  __)| | | | |/ || |/ ___) _  |     string.hpp
 * | |   | | |_| ( (_| | |  ( ( | |     Basic string operations.
 * |_|   |_|\__  |\____|_|   \_||_|         Will be removed soon!
 *         (____/
 */

#pragma once
#include <stdint.h>

typedef char* string; 

int64_t _strlen(const char *str);
const char* to_string(uint64_t value);
const char* to_string(int64_t value);
const char* to_hstring(uint64_t value);
const char* to_hstring(uint32_t value);
const char* to_hstring(uint16_t value);
const char* to_hstring(uint8_t value);
const char* to_string(double value, uint8_t decimalPlaces);
const char* to_string(double value);
const char* to_memstring(uint64_t value);
const char* to_memstring_type(uint64_t value);