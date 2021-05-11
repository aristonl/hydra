/*
 * Copyright Null LLC
 * Please read the License!
 *  _     _           _ 
 * | |   | |         | |
 * | |__ | |_   _  _ | | ____ ____
 * |  __)| | | | |/ || |/ ___) _  |     main.cpp
 * | |   | | |_| ( (_| | |  ( ( | |     Absolute beginning of kernel.
 * |_|   |_|\__  |\____|_|   \_||_|
 *         (____/
 */

#include "graphics/graphics.hpp"
#include "misc/string.hpp"
#include "misc/math.hpp"
#include "misc/bootloader.hpp"
#include "memory/memory.hpp"
#include "drivers/GDT/gdt.hpp"
#include "drivers/interrupts/idt.hpp"
#include "drivers/interrupts/interrupts.hpp"
#include "drivers/interrupts/mouse.hpp"
#include "memory/memory.hpp"
#include "misc/InputOutput.hpp"

extern uint64_t KernelStart;
extern uint64_t KernelEnd;

PageTableManager pageTableManager = NULL;

IDTR idtr;
void SetIDTGate(void* handler, uint8_t entryOffset, uint8_t type_attr, uint8_t selector) {
    IDTDescEntry* interrupt = (IDTDescEntry*)(idtr.Offset + entryOffset * sizeof(IDTDescEntry));
    interrupt->SetOffset((uint64_t)handler);
    interrupt->type_attr = type_attr;
    interrupt->selector = selector;
}

bool isInitialized = false;

void initialize(BootData* bootdata) {
    isInitialized = true;
    graphics->SetFramebuffer(bootdata->framebuffer);
    graphics->SetFont(bootdata->font);
    graphics->SetXY(0,0);
    graphics->SetColor(0xFFFFFF);
    GDTDescriptor gdtDescriptor;
    gdtDescriptor.Size = sizeof(GDT)-1;
    gdtDescriptor.Offset = (uint64_t) &DefaultGDT;
    LoadGDT(&gdtDescriptor);
    uint64_t mMapEntries = bootdata->MapSize / bootdata->MapDescriptorSize;
    Allocator = PageFrameAllocator();
    Allocator.ReadEFIMemoryMap(bootdata->Map, bootdata->MapSize, bootdata->MapDescriptorSize);
    uint64_t kernelSize = (uint64_t) &KernelEnd - (uint64_t) &KernelStart;
    uint64_t kernelPages = (uint64_t) kernelSize / 4096 + 1;
    Allocator.LockPages(&KernelStart, kernelPages);
    PageTable* PML4 = (PageTable*) Allocator.RequestPage();
    memset(PML4, 0, 0x1000);
    pageTableManager = PageTableManager(PML4);
    for (uint64_t t = 0; t < getMemorySize(bootdata->Map, mMapEntries, bootdata->MapDescriptorSize); t+= 0x1000) pageTableManager.MapMemory((void*) t, (void*) t);
    uint64_t fbBase = (uint64_t) bootdata->framebuffer->BaseAddress;
    uint64_t fbSize = (uint64_t) bootdata->framebuffer->Size + 0x1000;
    Allocator.LockPages((void*) fbBase, fbSize/ 0x1000 + 1);
    for (uint64_t t = fbBase; t < fbBase + fbSize; t += 4096) pageTableManager.MapMemory((void*) t, (void*) t);
    asm ("mov %0, %%cr3" : : "r" (PML4));
    idtr.Limit = 0x0FFF;
    idtr.Offset = (uint64_t) Allocator.RequestPage();
    SetIDTGate((void*) PageFaultHandler, 0xE, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*) DoublePageFaultHandler, 0x8, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*) GeneralPageFaultHandler, 0xD, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*) KeyboardHandler, 0x21, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*) MouseHandler, 0x2C, IDT_TA_InterruptGate, 0x08);
    asm ("lidt %0" : : "m" (idtr));
    RemapPIC();
    InitPS2Mouse();
    outb(PIC1_DATA, 0b11111001);
    outb(PIC2_DATA, 0b11101111);
    asm ("sti");
    memset(bootdata->framebuffer->BaseAddress, 0, bootdata->framebuffer->Size);
}

extern "C" int main(BootData* bootdata) {
    initialize(bootdata);
    while(true) ProcessMousePacket();
    return 300;
}