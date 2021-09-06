#include "bob.hpp"

extern "C" unsigned long long boot(void* ImageHandle, SystemTable* SystemTable) {
  SystemTable->ConOut->Reset(SystemTable->ConOut, 1);
  SystemTable->ConOut->OutputString(SystemTable->ConOut, (unsigned short int*) L"Better Opensource Bootloader (BOB)\r\n");
  SystemTable->BootServices->Stall(5000000);
  SystemTable->BootServices->SetWatchdogTimer(0, 0, 0, 0);
  return 0;
}