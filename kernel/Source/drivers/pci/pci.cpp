//========= Copyright N11 Software, All rights reserved. ============//
//
// File: pci.cpp
// Purpose:
// Maintainer: aristonl
//
//===================================================================//

#include <drivers/pci/pci.h>
#include <drivers/nvme/nvme.h>
#include <log.h>
#include <stdint.h>

namespace PCI {
	uint16_t get_vendor_id(uint16_t bus, uint16_t dev, uint16_t func) {
		uint32_t r0 = PCI::read_word(bus, dev, func, 0);
		return r0;
	}

	uint16_t get_device_id(uint16_t bus, uint16_t dev, uint16_t func) {
		uint32_t r0 = PCI::read_word(bus, dev, func, 2);
		return r0;
	}

	uint16_t get_class_id(uint16_t bus, uint16_t dev, uint16_t func) {
		uint32_t r0 = PCI::read_word(bus, dev, func, 0xA);
		return (r0 & ~0x00FF) >> 8;
	}

	uint16_t get_subclass_id(uint16_t bus, uint16_t dev, uint16_t func) {
		uint32_t r0 = PCI::read_word(bus, dev, func, 0xA);
		return (r0 & ~0xFF00);
	}

	void check_device(uint16_t bus, uint16_t dev) {
		uint16_t vendor_id = PCI::get_vendor_id(bus, dev, 0);
		uint16_t device_id = PCI::get_device_id(bus, dev, 0);
		uint16_t class_id = PCI::get_class_id(bus, dev, 0);
		uint16_t subclass_id = PCI::get_subclass_id(bus, dev, 0);
		if (vendor_id == 0xFFFF) return; /* Device doesn't exist */

		pci_device_t device;

		device.vendor_id = vendor_id;
		device.device_id = device_id;
		device.class_id = class_id;
		device.subclass_id = subclass_id;

		prDebug("PCI", "Detected device: %d:%d (Class %d, Subclass %d)",
				vendor_id, device_id, class_id, subclass_id);
	}

	void check_bus(uint16_t bus) {
		uint16_t device;

		for (device = 0; device < 32; device++) {
			PCI::check_device(bus, device);
		}
	}

	void init() {
		prInfo("PCI", "PCI Initialized");
		prInfo("PCI", "Using configuration mode 1 by default");
		PCI::check_bus(0);
	}
}
