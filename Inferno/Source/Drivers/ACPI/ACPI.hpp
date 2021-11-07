#pragma once

namespace ACPI {
  struct RSDP2 {
    unsigned char Signature[8], Checksum, OEMID[6], Revision;
    unsigned int RSDTAddress, Length;
    unsigned long long XSDTAddress;
    unsigned char ExtendedChecksum, Reserved[3];
  } __attribute__((packed));
  struct SDTHeader {
    unsigned char Signature[4];
    unsigned int Length;
    unsigned char Revision, Checksum, OEMID[6], OEMTableID[8];
    unsigned int OEMRevision, CreatorID, CreatorRevision;
  } __attribute__((packed));
  struct MCFGHeader {
    SDTHeader Header;
    unsigned long long Reserved;
  } __attribute__((packed));
  struct DeviceConfig {
    unsigned long long Address;
    unsigned short PCISegmentGroup;
    unsigned char StartBus, EndBus;
    unsigned int Reserved;
  } __attribute__((packed));
  void* FindTable(SDTHeader* header, char* signature);
}