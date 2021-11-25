#include "AHCI.hpp"
#include "../Graphics/GOP/GOP.hpp"
#include "../Memory/Memory.hpp"

namespace AHCI {
  PortType CheckPortType(HostBusAdapterPort* Port) {
    uint32_t SATAStatus = Port->SATAStatus;
    uint8_t InterfacePowerManagement = (SATAStatus >> 8) & 0b111;
    uint8_t DeviceDetection = SATAStatus & 0b111;
    if (DeviceDetection != 0x3) return PortType::None;
    if (InterfacePowerManagement != 0x1) return PortType::None;
    switch (Port->Signature){
      case 0xEB140101: return PortType::SATAPI;
      case 0x00000101: return PortType::SATA;
      case 0x96690101: return PortType::PM;
      case 0xC33C0101: return PortType::SEMB;
      default: PortType::None;
    }
    return PortType::None;
  }
  AHCIDriver::AHCIDriver(PCI::PCIDeviceHeader* Address) {
    this->Address = Address;
    ABAR = (HostBusAdapterMemory*)(unsigned long long)((PCI::PCIHeader0*)Address)->BAR5;
    pageTableManager.MapMemory(ABAR, ABAR);
    Probe();
    for (int i=0;i<PortCount;i++) {
      Port* port = Ports[i];
      port->Configure();
    }
  }
  AHCIDriver::~AHCIDriver() {}
  void AHCIDriver::Probe() {
    unsigned int PortsImplemented = ABAR->PortsImplemented;
    for (int i = 0; i < 32; i++) {
      if (PortsImplemented & (1<<i)) {
        PortType PortType = CheckPortType(&ABAR->Ports[i]);
        if (PortType == PortType::SATA || PortType == PortType::SATAPI) {
          Ports[PortCount] = new Port();
          Ports[PortCount]->Type = PortType;
          Ports[PortCount]->Port = &ABAR->Ports[i];
          Ports[PortCount]->number = i;
          PortCount++;
        }
      }
    }
  }
  void Port::Configure() {
    StopCommand();
    void* newBase = Allocator.RequestPage();
    this->Port->CommandList = (uint32_t)(uint64_t)newBase;
    this->Port->CommandListUpper = (uint32_t)((uint64_t)newBase >> 32);
    memset((void*)((unsigned long long) this->Port->CommandList), 0, 1024);
    void* FISAddress = Allocator.RequestPage();
    this->Port->FISAddress = (uint32_t)(uint64_t)FISAddress;
    this->Port->FISAddressUpper = (uint32_t)((uint64_t)FISAddress >> 32);
    memset(FISAddress, 0, 256);
    HostBusAdapterCommandHeader* CommandHeader = (HostBusAdapterCommandHeader*)((uint64_t)this->Port->CommandList + ((uint64_t)this->Port->CommandListUpper << 32));
    for (int i = 0; i < 32; i++){
      CommandHeader[i].prdtLength = 8;
      void* cmdTableAddress = Allocator.RequestPage();
      uint64_t address = (uint64_t)cmdTableAddress + (i << 8);
      CommandHeader[i].commandTableBaseAddress = (uint32_t)(uint64_t)address;
      CommandHeader[i].commandTableBaseAddressUpper = (uint32_t)((uint64_t)address >> 32);
      memset(cmdTableAddress, 0, 256);
    }
    StartCommand();
  }
  void Port::StopCommand() {
    this->Port->CommandStatus &= ~0x0001;
    this->Port->CommandStatus &= ~0x0010;
    while(true) {
      if (this->Port->CommandStatus & 0x8000) continue;
      if (this->Port->CommandStatus & 0x8000) continue;
      break;
    }
  }
  void Port::StartCommand() {
    while (this->Port->CommandStatus & 0x80000);
    this->Port->CommandStatus |= 0x0010;
    this->Port->CommandStatus |= 0x0001;
  }
}