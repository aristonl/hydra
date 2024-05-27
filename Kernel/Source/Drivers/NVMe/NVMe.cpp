#include <drivers/nvme/nvme.h>
#include <drivers/pci/pci.h>
#include <log.h>

#include <stdint.h>

int nvme_init(uint16_t vendor_id, uint16_t device_id) {
	if (vendor_id == 0x1b36 && device_id == 0x0010) {
		prInfo("nvme", "Initializing NVMe device, QEMU NVM Express Controller");
	}

	return 0;
}
