#include <Config.hpp>
#include <IO.hpp>
#include <COM.hpp>
#include <GDT.hpp>
#include <Interrupts.hpp>

__attribute__((interrupt)) void test(void*) {
	kprintf("The test interrupt was requested successfully!\n\r");
}

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
	Interrupts::CreateISR(0, (void*)test);

	// Load IDT
	Interrupts::EnableInterrupts();
	kprintf("\r\e[92m[INFO] Loaded IDT...\e[0m\n\r");

	// IRQ Test
	asm volatile("int $0x0");
}

__attribute__((ms_abi)) [[noreturn]] void main() {
  InitializeSerialDevice();
  Inferno();
  
  // Once finished say hello and halt
  kprintf("\e[92m[INFO] Done!\e[0m\n\r");

  while(true) asm("hlt");
}
