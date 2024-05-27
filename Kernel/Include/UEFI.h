//========= Copyright N11 Software, All rights reserved. ============//
//
// File: UEFI.h
// Purpose: UEFI/EFI Header. Used for EFI Boot stub
// Maintainer: aristonl
//
//===================================================================//

/*
 * Once I'm able to get all UEFI things implemented, we should be able to
 * start working on the UEFI boot stub which will let us boot without BOB
 * or any other bootloader.
 */

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t EFI_STATUS;

#define EFI_RUNTIME_SERVICES 3


#ifdef __cplusplus
}
#endif
