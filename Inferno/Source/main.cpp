#include "Config.hpp"
#include "Drivers/IO/IO.hpp"
#include "Drivers/COM/COM.hpp"
#include "Drivers/GDT/GDT.hpp"

extern unsigned long long InfernoStart;
extern unsigned long long InfernoEnd;

__attribute__((sysv_abi)) void Inferno() {
  // Enable GDT
  #if EnableGDT == true
    __attribute__((aligned(0x1000)))
    GDT::Table Table = {
      {0, 0, 0, 0x00, 0x00, 0},
      {0, 0, 0, 0x9a, 0xa0, 0},
      {0, 0, 0, 0x92, 0xa0, 0},
      {0, 0, 0, 0xfa, 0xa0, 0},
      {0, 0, 0, 0xf2, 0xa0, 0},
    };
    GDT::Descriptor descriptor;
    descriptor.size = sizeof(Table)-1;
    descriptor.offset = (unsigned long long)&Table;
    LoadGDT(&descriptor);
    kprintf("Loaded GDT...\n");
  #endif
}

__attribute__((ms_abi)) void main() {
  InitializeSerialDevice();
  Inferno();
  
  // Once finished say hello and halt
  kprintf("Hello, world!\n");

  while(true) asm("hlt");
}