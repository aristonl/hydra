#include "PCI.hpp"

namespace PCI {
  void EnumerateFunction(unsigned long long Address, unsigned long long function) {
    unsigned long long offset = function << 12;
    unsigned long long functionAddress = Address + offset;
    pageTableManager.MapMemory((void*)functionAddress, (void*)functionAddress);
    PCIDeviceHeader* DeviceHeader = (PCIDeviceHeader*)functionAddress;
    if (DeviceHeader->DeviceID == 0) return;
    if (DeviceHeader->DeviceID == 0xFFFF) return;
    kprintf("0x");
    kprintf(to_hstring(DeviceHeader->VendorID));
    kprintf(" - 0x");
    kprintf(to_hstring(DeviceHeader->DeviceID));
    kprintf("\n");
  }
  void EnumerateDevice(unsigned long long Address, unsigned long long device) {
    unsigned long long offset = device << 15;
    unsigned long long deviceAddress = Address + offset;
    pageTableManager.MapMemory((void*)deviceAddress, (void*)deviceAddress);
    PCIDeviceHeader* DeviceHeader = (PCIDeviceHeader*)deviceAddress;
    if (DeviceHeader->DeviceID == 0) return;
    if (DeviceHeader->DeviceID == 0xFFFF) return;
    for (unsigned long long function=0;function<8;function++) {
      EnumerateFunction(deviceAddress, function);
    }
  }
  void EnumerateBus(unsigned long long Address, unsigned long long bus) {
    unsigned long long offset = bus << 20;
    unsigned long long busAddress = Address + offset;
    pageTableManager.MapMemory((void*)busAddress, (void*)busAddress);
    PCIDeviceHeader* DeviceHeader = (PCIDeviceHeader*)busAddress;
    if (DeviceHeader->DeviceID == 0) return;
    if (DeviceHeader->DeviceID == 0xFFFF) return;
    for (unsigned long long device=0;device<32;device++) {
      EnumerateDevice(busAddress, device);
    }
  }
  void EnumeratePCI(ACPI::MCFGHeader* mcfg) {
    int entries = ((mcfg->Header.Length)-sizeof(ACPI::MCFGHeader))/sizeof(ACPI::DeviceConfig);
    for (int t=0;t<entries;t++) {
      ACPI::DeviceConfig* device = (ACPI::DeviceConfig*)((unsigned long long)mcfg+sizeof(ACPI::MCFGHeader)+(sizeof(ACPI::DeviceConfig)*t));
      for (unsigned long long bus = device->StartBus;bus<device->EndBus;bus++) EnumerateBus(device->Address, bus);
    }
  }
}