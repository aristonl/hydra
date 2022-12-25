//========= Copyright N11 Software, All rights reserved. ============//
//
// File: main.cpp
// Purpose: Main point of the kernel.
// Maintainer: aristonl, FiReLScar
//
//===================================================================//

#include <unistd.h>
#include <stdint.h>

#include <BOB.hpp>
#include <COM.hpp>
#include <Config.hpp>
#include <GDT.hpp>
#include <sys/io.h>
#include <Interrupts/APIC.hpp>
#include <Interrupts/DoublePageFault.hpp>
#include <Interrupts/Interrupts.hpp>
#include <Interrupts/PageFault.hpp>
#include <Interrupts/Syscall.hpp>
#include <Memory/Memory.hpp>
#include <Memory/Mem_.hpp>
#include <CPU/CPUID.hpp>

// Drivers
#include <drivers/rtc/rtc.h>

extern unsigned long long InfernoStart;
extern unsigned long long InfernoEnd;

__attribute__((sysv_abi)) void Inferno(BOB* bob) {
	// Memory
	Memory::Init(bob);
	kprintf("\e[92m[INFO] Total Memory: %dMB\n\r", Memory::GetSize()/0x1000/1024);

	// Create GDT
	#if EnableGDT == true
		GDT::Table GDT = {
			{ 0, 0, 0, 0x00, 0x00, 0 },
			{ 0, 0, 0, 0x9a, 0xa0, 0 },
			{ 0, 0, 0, 0x92, 0xa0, 0 },
			{ 0, 0, 0, 0xfa, 0xa0, 0 },
			{ 0, 0, 0, 0xf2, 0xa0, 0 },
		};
		GDT::Descriptor descriptor;
		descriptor.size = sizeof(GDT) - 1;
		descriptor.offset = (unsigned long long)&GDT;
		LoadGDT(&descriptor);
		kprintf("\r\e[92m[INFO] Loaded GDT...\e[0m\n\r");
	#endif

    // Create IDT
    Interrupts::CreateIDT();

    // Create a test ISR
    Interrupts::CreateISR(0x80, (void*)SyscallHandler);
    Interrupts::CreateISR(0x0E, (void*)PageFault);
    Interrupts::CreateISR(0x08, (void*)DoublePageFault);

    // Load APIC
    if (APIC::Capable()) {
        APIC::Enable();
        kprintf("\r\e[92m[INFO] Loaded APIC...\e[0m\n\r");
    }

    // Load IDT
    Interrupts::LoadIDT();
    kprintf("\r\e[92m[INFO] Loaded IDT...\e[0m\n\r");

    unsigned long res = 0;
    asm volatile("int $0x80" : "=a"(res) : "a"(1), 
			"d"((unsigned long)"Hello from syscall\n\r"), 
			"D"((unsigned long)0) : "rcx", "r11", "memory");

	RTC::init();
	
}

__attribute__((ms_abi)) [[noreturn]] void main(BOB* bob) {
    Inferno(bob);

    // Once finished say hello and halt
    kprintf("\e[92m[INFO] Done!\e[0m\n\r");

    while (true) asm("hlt");
}
