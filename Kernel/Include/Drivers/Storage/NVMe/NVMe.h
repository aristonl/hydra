#pragma once

#include <Drivers/PCI/PCI.h>

#include <stdint.h>

int nvme_init(uint16_t vendor_id, uint16_t device_id);
