/* Copyright (c) 2021 Null. All rights reserved.
 * Copyright (c) 2021 Echo/Inferno/B.O.B. Developers.
 */
/*
 * Copyright (c) 2020 Optix.
 * Licensed under the Optix Public License.
 */
/*
 * cdefs.h
 *
 * Created by Ariston Lorenzo on 1/9/2020.
 * Copyright 2020 Ariston Lorenzo.
 */

#pragma once

#define _POSIX_VERSION 200809L

#ifndef ALWAYS_INLINE
#    define ALWAYS_INLINE inline __attribute__((always_inline))
#endif

#ifdef __cplusplus
#    ifndef __BEGIN_DECLS
#        define __BEGIN_DECLS extern "C" {
#        define __END_DECLS }
#    endif
#else
#    ifndef __BEGIN_DECLS
#        define __BEGIN_DECLS
#        define __END_DECLS
#    endif
#endif

#undef __P
#define __P(a) a

#define offsetof(type, member) __builtin_offsetof(type, member)

#define __n11_libc 1

#ifdef __cplusplus
// extern "C" int main(int, char**);
#endif
