#pragma once
#include "../PCI/PCI.hpp"

namespace AHCI {
  enum PortType { None = 0, SATA = 1, SEMB = 2, PM = 3, SATAPI = 4 };
  struct HostBusAdapterPort{
    unsigned int CommandList, CommandListUpper, FISAddress, FISAddressUpper, InterruptStatus, InterruptEnable, CommandStatus, Reserved0, TaskFileData, Signature, SATAStatus, SATAControl, SATAError, SATAActive, CommandIssue, SATANotification, FISSwitchControl, Reserved1[11], Vendor[4];
  };
  struct HostBusAdapterMemory{
    unsigned int HostCapability, GlobalHostControl, InterruptStatus, PortsImplemented, Version, CCCControl, CCCPorts, EnclosureManagementLocation, EnclosureManagementControl, HostCapabilitiesExtended, BIOSHandoffControlStatus;
    unsigned char Reserved[0x74], Vendor[0x60];
    HostBusAdapterPort Ports[1];
  };
  struct HostBusAdapterCommandHeader {
    unsigned char commandFISLength:5, atapi:1, write:1, prefetchable:1, reset:1, bist:1, clearBusy:1, rsv0:1, portMultiplier:4;
    unsigned short prdtLength;
    unsigned int prdbCount, commandTableBaseAddress, commandTableBaseAddressUpper, rsv1[4];
  };
  class Port {
    public:
      HostBusAdapterPort* Port;
      PortType Type;
      unsigned char* buffer, number;
      void Configure();
      void StopCommand();
      void StartCommand();
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