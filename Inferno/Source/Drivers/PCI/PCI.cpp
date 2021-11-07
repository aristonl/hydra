#include "PCI.hpp"
#include "../../Graphics/Font/Font.hpp"

const char* ClassMap[] = {
  "Unclassified", "Mass Storage Controller", "Network Controller", "Display Controller", "Multimedia Controller", "Memory Controller", "Bridge Device",
  "Simple Communication Controller", "Base System Peripheral", "Input Device Controller", "Docking Station", "Processor", "Serial Bus Controller", "Wireless Controller",
  "Intelligent Controller", "Satellite Communication Controller", "Encryption Controller", "Signal Processing Controller", "Processing Accelerator", "Non Essential Instrumentation"
};
const char* GetVendorName(uint16_t vendorID) {
  switch (vendorID){
    case 0x8086: return "Intel";
    case 0x1022: return "AMD";
    case 0x10DE: return "NVIDIA";
  }
  return "Unknown";
}
const char* GetDeviceName(uint16_t vendorID, uint16_t deviceID) {
  switch (vendorID) {
    case 0x8086:
      switch(deviceID){
        case 0x29C0: return "Express DRAM Controller";
        case 0x2918: return "LPC Interface Controller";
        case 0x2922: return "6 port SATA Controller [AHCI mode]";
        case 0x2930: return "SMBus Controller";
      }
  }
  return "Unknown";
}
const char* MassStorageControllerSubclassName(uint8_t subclassCode) {
  switch (subclassCode){
    case 0x00: return "SCSI Bus Controller";
    case 0x01: return "IDE Controller";
    case 0x02: return "Floppy Disk Controller";
    case 0x03: return "IPI Bus Controller";
    case 0x04: return "RAID Controller";
    case 0x05: return "ATA Controller";
    case 0x06: return "Serial ATA";
    case 0x07: return "Serial Attached SCSI";
    case 0x08: return "Non-Volatile Memory Controller";
    case 0x80: return "Other";
  }
  return "Unknown";
}
const char* SerialBusControllerSubclassName(uint8_t subclassCode) {
  switch (subclassCode){
    case 0x00: return "FireWire (IEEE 1394) Controller";
    case 0x01: return "ACCESS Bus";
    case 0x02: return "SSA";
    case 0x03: return "USB Controller";
    case 0x04: return "Fibre Channel";
    case 0x05: return "SMBus";
    case 0x06: return "Infiniband";
    case 0x07: return "IPMI Interface";
    case 0x08: return "SERCOS Interface (IEC 61491)";
    case 0x09: return "CANbus";
    case 0x80: return "SerialBusController - Other";
  }
  return "Unknown";
}
const char* BridgeDeviceSubclassName(uint8_t subclassCode) {
  switch (subclassCode){
    case 0x00: return "Host Bridge";
    case 0x01: return "ISA Bridge";
    case 0x02: return "EISA Bridge";
    case 0x03: return "MCA Bridge";
    case 0x04: return "PCI-to-PCI Bridge";
    case 0x05: return "PCMCIA Bridge";
    case 0x06: return "NuBus Bridge";
    case 0x07: return "CardBus Bridge";
    case 0x08: return "RACEway Bridge";
    case 0x09: return "PCI-to-PCI Bridge";
    case 0x0a: return "InfiniBand-to-PCI Host Bridge";
    case 0x80: return "Other";
  }
  return "Unknown";
}
const char* GetSubclassName(uint8_t classCode, uint8_t subclassCode) {
  switch (classCode){
      case 0x01: return MassStorageControllerSubclassName(subclassCode);
      case 0x03:
        switch (subclassCode){
          case 0x00: return "VGA Compatible Controller";
        }
      case 0x06: return BridgeDeviceSubclassName(subclassCode);
      case 0x0C: return SerialBusControllerSubclassName(subclassCode);
  }
  return "Unknown";
}
const char* GetProgramInterfaceName(uint8_t classCode, uint8_t subclassCode, uint8_t progIF) {
  switch (classCode){
    case 0x01:
      switch (subclassCode){
        case 0x06:
          switch (progIF){
            case 0x00: return "Vendor Specific Interface";
            case 0x01: return "AHCI 1.0";
            case 0x02: return "Serial Storage Bus";
          }
      }
    case 0x03:
      switch (subclassCode){
        case 0x00:
          switch (progIF){
            case 0x00: return "VGA Controller";
            case 0x01: return "8514-Compatible Controller";
          }
      }
    case 0x0C:
      switch (subclassCode){
        case 0x03:
          switch (progIF){
            case 0x00: return "UHCI Controller";
            case 0x10: return "OHCI Controller";
            case 0x20: return "EHCI (USB2) Controller";
            case 0x30: return "XHCI (USB3) Controller";
            case 0x80: return "Unspecified";
            case 0xFE: return "USB Device (Not a Host Controller)";
          }
      }
    }
  return "Unknown";
}

namespace PCI {
  void EnumerateFunction(unsigned long long Address, unsigned long long function) {
    unsigned long long offset = function << 12;
    unsigned long long functionAddress = Address + offset;
    pageTableManager.MapMemory((void*)functionAddress, (void*)functionAddress);
    PCIDeviceHeader* DeviceHeader = (PCIDeviceHeader*)functionAddress;
    if (DeviceHeader->DeviceID == 0) return;
    if (DeviceHeader->DeviceID == 0xFFFF) return;
    printf(GetVendorName(DeviceHeader->VendorID));
    printf(" - ");
    printf(GetDeviceName(DeviceHeader->VendorID, DeviceHeader->DeviceID));
    printf(" - ");
    printf(ClassMap[DeviceHeader->Class]);
    printf(" - ");
    printf(GetSubclassName(DeviceHeader->Class, DeviceHeader->SubClass));
    printf(" - ");
    printf(GetProgramInterfaceName(DeviceHeader->Class, DeviceHeader->SubClass, DeviceHeader->ProgramInterface));
    printf("\n");
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