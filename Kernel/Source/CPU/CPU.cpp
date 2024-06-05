//========= Copyright N11 Software, All rights reserved. ============//
//
// File: CPU.cpp
// Purpose: The Inferno kernel source code style guide.
// Maintainer: atl
//
//===================================================================//

#include <Log.h>
#include <CPU/CPU.h>
#include <CPU/CPUID.h>
#include <CPU/VendorID.h>

#include <string.h>

namespace CPU {
	void CPUDetect() {
		const char *vendor = VendorID();
		if (vendor == CPUID_VENDOR_INTEL) {
			IntelHandler();
		} else if (vendor == CPUID_VENDOR_AMD || vendor == CPUID_VENDOR_AMD_ALT) {
			// AMDHandler();
		} else {
			// UnknownHandler();
		}
	}

	void IntelHandler() {
		/* determine the model */
		char *model = Model();
		prDebug("cpu", "Intel CPU detected: %s", model);
	}
}