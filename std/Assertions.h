//
// Created by ariston on 2021/10/06.
//

#pragma once

#if defined(KERNEL)
#    include <Inferno/Assertions.h>
#else
#    include <assert.h>
#    define VERIFY assert
#    define VERIFY_NOT_REACHED() assert(false)
static constexpr bool TODO = false;
#    define TODO() VERIFY(TODO)
#endif