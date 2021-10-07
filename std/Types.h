#pragma once

#include "../Libraries/LibC/stdint.h"

typedef uint8_t byte;
typedef uint16_t word;
typedef uint32_t dword;
typedef uint64_t qword;

typedef int8_t signed_byte;
typedef int16_t signed_word;
typedef int32_t signed_dword;
typedef int64_t signed_qword;

typedef unsigned long long size_t;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

constexpr unsigned KB = 1024;
constexpr unsigned MB = KB * KB;
constexpr unsigned GB = KB * KB * KB;
