//========= Copyright N11 Software, All rights reserved. ============//
//
// File: main.cpp
// Purpose: Main point of the kernel.
// Maintainer: aristonl, FiReLScar
//
//===================================================================//

#include <unistd.h>
#include <stdint.h>

#include <Boot/BOB.h>
#include <Drivers/TTY/COM.h>
#include <Inferno/Config.h>
#include <CPU/CPU.h>
#include <CPU/GDT.h>
#include <sys/io.h>
#include <Interrupts/APIC.hpp>
#include <Interrupts/DoublePageFault.hpp>
#include <Interrupts/Interrupts.hpp>
#include <Interrupts/PageFault.hpp>
#include <Interrupts/Syscall.hpp>
#include <Memory/Memory.hpp>
#include <Memory/Mem_.hpp>
#include <Memory/Paging.h>
#include <CPU/CPUID.h>
#include <Inferno/Log.h>
#include <Drivers/TTY/TTY.h>

// Drivers
#include <Drivers/RTC/RTC.h>
#include <Drivers/PCI/PCI.h>

extern unsigned long long InfernoStart;
extern unsigned long long InfernoEnd;

__attribute__((sysv_abi)) void Inferno(BOB* bob) {
	prInfo("kernel", "Inferno kernel version 0.1alpha");
	kprintf("Copyright 2021-2024 N11 Software.\nCopyright 2018-2021 Ariston Lorenzo and Levi Hicks. All rights reserved.\nSee COPYRIGHT in the Inferno source tree for more information.\n");

	RTC::init();

	// CPU
	CPU::CPUDetect();

	// Memory
	Memory::Init(bob);
	unsigned long long memory = Memory::GetFree();
	if (memory < 1073741824 && memory > 269484032) memory -= 269484032;
	prInfo("mm", "Total Memory: %M", memory);

	unsigned long long kSize = (unsigned long long)&InfernoEnd-(unsigned long long)&InfernoStart;
	Paging::AllocatePages(&InfernoStart, kSize/1024+1);

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
		prInfo("kernel", "initalized GDT");
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
			prInfo("apic", "initalized APIC");
	}

	// Load IDT
	Interrupts::LoadIDT();
	prInfo("idt", "initialized IDT");

	unsigned long res = 0;
	asm volatile("int $0x80" : "=a"(res) : "a"(1), 
		"d"((unsigned long)"Hello from syscall\n\r"), 
		"D"((unsigned long)0) : "rcx", "r11", "memory");

	PCI::init();
}

__attribute__((ms_abi)) [[noreturn]] void main(BOB* bob) {
	SetFramebuffer(bob->framebuffer);
	SetFont(bob->FontFile);
	Inferno(bob);
	unsigned long long timeNow = RTC::getEpochTime();
	test();
	unsigned long long render = RTC::getEpochTime()-timeNow;
	prInfo("kernel", "Render time: %ds", render);

	// Once finished say hello and halt
	prInfo("kernel", "Done!");

	while (true) asm("hlt");
}
