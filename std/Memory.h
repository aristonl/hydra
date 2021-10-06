//
// Created by ariston on 2021/10/06.
//

#pragma once

#include "Types.h"

#include "../Libraries/LibC/stdlib.h"
#include "../Libraries/LibC/string.h"

ALWAYS_INLINE void fast_u32_copy(u32* dest, const u32* src, size_t count)
{
#if ARCH(X64)
    asm volatile(
        "rep movsl\n"
        : "+S"(src), "+D"(dest), "+c"(count)::"memory");
#else
    __builtin_memcpy(dest, src, count * 4);
#endif
}

ALWAYS_INLINE void fast_u32_fill(u32* dest, u32 value, size_t count)
{
#if ARCH(X64)
    asm volatile(
        "rep stosl\n"
        : "=D"(dest), "=c"(count)
        : "D"(dest), "c"(count), "a"(value)
        : "memory");
#else
    for (auto* p = dest; p < (dest + count); ++p) {
        *p = value;
    }
#endif
}

namespace std {
    inline void secure_zero(void* ptr, size_t size)
    {
        __builtin_memset(ptr, 0, size);
        // The memory barrier is here to avoid the compiler optimizing
        // away the memset when we rely on it for wiping secrets.
        asm volatile("" ::
        : "memory");
    }
}

using std::secure_zero;