#pragma once
#include "../PCI/PCI.hpp"

namespace AHCI {
  enum FISType { FISHD = 0x27, FISDH = 0x34, FISACT = 0x39, FISDMA = 0x41, FISData = 0x46, FISBIST = 0x58, FISPIO = 0x5F, FISDeviceBits = 0xA1 };
  enum PortType { None = 0, SATA = 1, SEMB = 2, PM = 3, SATAPI = 4 };
  struct HostBusAdapterPort{
        uint32_t CommandList;
        uint32_t CommandListUpper;
        uint32_t FISAddress;
        uint32_t FISAddressUpper;
        uint32_t InterruptStatus;
        uint32_t InterruptEnable;
        uint32_t CommandStatus;
        uint32_t rsv0;
        uint32_t TaskFileData;
        uint32_t Signature;
        uint32_t SATAStatus;
        uint32_t SATAControl;
        uint32_t SATAError;
        uint32_t SATAActive;
        uint32_t CommandIssue;
        uint32_t SATANotification;
        uint32_t FISSwitchControl;
        uint32_t rsv1[11];
        uint32_t vendor[4];
    };

    struct HostBusAdapterMemory{
        uint32_t HostCapability;
        uint32_t GlobalHostControl;
        uint32_t InterruptStatus;
        uint32_t PortsImplemented;
        uint32_t Version;
        uint32_t CCCControl;
        uint32_t CCCPorts;
        uint32_t EnclosureManagementLocation;
        uint32_t EnclosureManagementControl;
        uint32_t HostCapabilitiesExtended;
        uint32_t BIOSHandoffCtrlSts;
        uint8_t rsv0[0x74];
        uint8_t Vendor[0x60];
        HostBusAdapterPort Ports[1];
    };
  struct HostBusAdapterCommandHeader {
    unsigned char commandFISLength:5, atapi:1, write:1, prefetchable:1, reset:1, bist:1, clearBusy:1, rsv0:1, portMultiplier:4;
    unsigned short prdtLength;
    unsigned int prdbCount, commandTableBaseAddress, commandTableBaseAddressUpper, rsv1[4];
  };
      struct FISRegisterHardwareToDevice {
        uint8_t Type;

        uint8_t PortMultiplier:4;
        uint8_t rsv0:3;
        uint8_t CommandControl:1;

        uint8_t Command;
        uint8_t FeatureLow;

        uint8_t LBA0;
        uint8_t LBA1;
        uint8_t LBA2;
        uint8_t DeviceRegister;

        uint8_t LBA3;
        uint8_t LBA4;
        uint8_t LBA5;
        uint8_t FeatureHigh;

        uint8_t CountLow;
        uint8_t CountHigh;
        uint8_t ISOCommandCompletion;
        uint8_t Control;

        uint8_t rsv1[4];
    };
  struct HostBusAdapterPRDTEntry{
    uint32_t DataBaseAddress;
    uint32_t DataBaseAddressUpper;
    uint32_t rsv0;

    uint32_t ByteCount:22;
    uint32_t rsv1:9;
    uint32_t InterruptOnCompletion:1;
  };
  struct HostBusAdapterCommandTable{
        uint8_t CommandFIS[64];

        uint8_t atapiCommand[16];

        uint8_t rsv[48];

        HostBusAdapterPRDTEntry PRDTEntry[];
    };
  class Port {
    public:
      HostBusAdapterPort* Port;
      PortType Type;
      unsigned char* buffer, number;
      void Configure();
      void StopCommand();
      void StartCommand();
      bool Read(unsigned long long sector, unsigned int count, void* buffer);
  };
  class AHCIDriver {
    public:
      AHCIDriver(PCI::PCIDeviceHeader* Address);
      ~AHCIDriver();
      void Probe();
      HostBusAdapterMemory* ABAR;
      PCI::PCIDeviceHeader* Address;
      Port* Ports[32];
      unsigned char PortCount;
  };
}