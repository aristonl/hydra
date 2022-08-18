#include <Config.hpp>
#include <IO.hpp>
#include <COM.hpp>
#include <GDT.hpp>
#include <Interrupts.hpp>
#include <Interrupts/Syscall.hpp>
#include <Interrupts/PageFault.hpp>
#include <Interrupts/DoublePageFault.hpp>

extern unsigned long long InfernoStart;
extern unsigned long long InfernoEnd;

__attribute__((sysv_abi)) void Inferno() {
	// Create GDT
	#if EnableGDT == true
		GDT::Table GDT = {
			{0, 0, 0, 0x00, 0x00, 0},
			{0, 0, 0, 0x9a, 0xa0, 0},
			{0, 0, 0, 0x92, 0xa0, 0},
			{0, 0, 0, 0xfa, 0xa0, 0},
			{0, 0, 0, 0xf2, 0xa0, 0},
		};
		GDT::Descriptor descriptor;
		descriptor.size = sizeof(GDT)-1;
		descriptor.offset = (unsigned long long)&GDT;
		LoadGDT(&descriptor);
		kprintf("\r\e[92m[INFO] Loaded GDT...\e[0m\n\r");
	#endif

	// Create IDT
	Interrupts::CreateIDT();

	// Create a test ISR
	Interrupts::CreateISR(0x80, (void*)Syscall);
	Interrupts::CreateISR(0x0E, (void*)PageFault);
	Interrupts::CreateISR(0x08, (void*)DoublePageFault);

	// Load IDT
	Interrupts::EnableInterrupts();
	kprintf("\r\e[92m[INFO] Loaded IDT...\e[0m\n\r");

	// IRQ Test
	asm volatile("int $0x80");
}

__attribute__((ms_abi)) [[noreturn]] void main() {
  InitializeSerialDevice();
  Inferno();
  
  // Once finished say hello and halt
  kprintf("\e[92m[INFO] Done!\e[0m\n\r");
  kprintf("String: %s\n\r", "Hello, world!");

  while(true) asm("hlt");
}
