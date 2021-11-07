#include "ACPI.hpp"

namespace ACPI {
  void* FindTable(SDTHeader* header, char* signature) {
    int entries = (header->Length - sizeof(ACPI::SDTHeader)) / 8;
    for (int t=0;t<entries;t++) {
      ACPI::SDTHeader* entry = (ACPI::SDTHeader*)*(unsigned long long*)((unsigned long long)header+sizeof(ACPI::SDTHeader)+(t*8));
      for (int i=0;i<4;i++) {
        if (entry->Signature[i] != signature[i]) break;
        if (i == 3) return entry;
      }
    }
    return (void*)0;
  }
}