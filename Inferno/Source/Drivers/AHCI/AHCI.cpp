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
      port->buffer = (unsigned char*) Allocator.RequestPage();
      memset(port->buffer, 0, 4096);
      if (port->Read(0, 4, port->buffer)) {
        for (int t=0;t<1024;t++) {
          printf((char)port->buffer[t]);
        }
        printf("\n");
      } else {
        printf("Failed to read disk!\n");
        return;
      }
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
          Ports[PortCount]->number = PortCount;
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
  bool Port::Read(unsigned long long sector, unsigned int count, void* buffer) {
    unsigned int sectorLow = (unsigned int)sector, sectorHigh = (unsigned int)(sector >> 32);
    this->Port->InterruptStatus = (unsigned int)-1;
    HostBusAdapterCommandHeader* CommandHeader = (HostBusAdapterCommandHeader*)(unsigned long long)(this->Port->CommandList);
    CommandHeader->commandFISLength = sizeof(FISRegisterHardwareToDevice)/sizeof(unsigned int);
    CommandHeader->write = 0;
    CommandHeader->prdtLength = 1;

    HostBusAdapterCommandTable* CommandTable = (HostBusAdapterCommandTable*)(unsigned long long) CommandHeader->commandTableBaseAddress;
    memset(CommandTable, 0, sizeof(HostBusAdapterCommandTable)+(CommandHeader->prdtLength-1)*sizeof(HostBusAdapterPRDTEntry));

    CommandTable->PRDTEntry[0].DataBaseAddress = (unsigned int)(unsigned long long)buffer;
    CommandTable->PRDTEntry[0].DataBaseAddressUpper = (unsigned int)((unsigned long long)buffer >> 32);
    CommandTable->PRDTEntry[0].ByteCount = (count<<9)-1;
    CommandTable->PRDTEntry[0].InterruptOnCompletion = 1;
    FISRegisterHardwareToDevice* CommandFIS = (FISRegisterHardwareToDevice*)&CommandTable->CommandFIS;
    CommandFIS->Type = 0x27;
    CommandFIS->Control = 1;
    CommandFIS->Command = 0x25;
    CommandFIS->LBA0 = (unsigned char) (sectorLow);
    CommandFIS->LBA1 = (unsigned char) (sectorLow>>8);
    CommandFIS->LBA2 = (unsigned char) (sectorLow>>16);
    CommandFIS->LBA3 = (unsigned char) (sectorHigh);
    CommandFIS->LBA4 = (unsigned char) (sectorHigh>>8);
    CommandFIS->LBA5 = (unsigned char) (sectorHigh>>16);
    CommandFIS->DeviceRegister = 1<<6;
    CommandFIS->CountLow = count&0xFF;
    CommandFIS->CountHigh = (count>>8)&0xFF;

    unsigned long long spin = 0;
    while(this->Port->TaskFileData & (0x80 | 0x08) && spin < 1000000) spin++;
    if (spin == 1000000) return false;

    Port->CommandIssue = 1;
    while (true) {
      if (this->Port->CommandIssue == 0) break;
      if (this->Port->InterruptStatus & (1<<30)) return false;
    }
    return true;
  }
}