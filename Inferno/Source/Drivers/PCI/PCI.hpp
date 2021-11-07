#pragma once
#include "../ACPI/ACPI.hpp"
#include "../Memory/Memory.hpp"
#include "../../Graphics/Font/Font.hpp"

namespace PCI {
  struct PCIDeviceHeader {
    unsigned short VendorID, DeviceID, Command, Status;
    unsigned char RevisionID, ProgramInterface, SubClass, Class, CacheLineSize, LatencyTimer, HeaderType, BIST;
  };
  void EnumeratePCI(ACPI::MCFGHeader* mcfg);
}