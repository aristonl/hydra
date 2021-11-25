#pragma once
#include "../ACPI/ACPI.hpp"
#include "../Memory/Memory.hpp"
#include "../../Graphics/Font/Font.hpp"
#include "../COM/COM.hpp"

namespace PCI {
  struct PCIDeviceHeader {
    unsigned short VendorID, DeviceID, Command, Status;
    unsigned char RevisionID, ProgramInterface, SubClass, Class, CacheLineSize, LatencyTimer, HeaderType, BIST;
  };
  struct PCIHeader0 {
    PCIDeviceHeader Header;
    unsigned int BAR0, BAR1, BAR2, BAR3, BAR4, BAR5, CardbusCISPointer;
    unsigned short SubsystemVendorID, SubsystemID;
    unsigned int ExpansionROMBaseAddr;
    unsigned char CapabilitiesPointer, Reserved0;
    unsigned short Reserved1;
    unsigned int Reserved2;
    unsigned char InterruptLine, InterruptPin, MinGrant, MaxLatency;
  };
  void EnumeratePCI(ACPI::MCFGHeader* mcfg);
}