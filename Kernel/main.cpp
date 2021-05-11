#include "graphics/graphics.h"
#include "misc/string.h"
#include "misc/math.h"
#include "misc/bootloader.h"
#include "memory/memory.h"
#include "drivers/interrupts/gdt.h"
#include "drivers/interrupts/idt.h"
#include "drivers/interrupts/interrupts.h"
#include "drivers/interrupts/mouse.h"
#include "memory/memory.h"
#include "misc/InputOutput.h"

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
    memset(bootdata->framebuffer->BaseAddress, 0, bootdata->framebuffer->Size);
    asm ("mov %0, %%cr3" : : "r" (PML4));
    idtr.Limit = 0x0FFF;
    idtr.Offset = (uint64_t) Allocator.RequestPage();
    SetIDTGate((void*) PageFault_Handler, 0xE, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*) DoubleFault_Handler, 0x8, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*) GPFault_Handler, 0xD, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*) KeyboardInt_Handler, 0x21, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*) MouseInt_Handler, 0x2C, IDT_TA_InterruptGate, 0x08);
    asm ("lidt %0" : : "m" (idtr));
    RemapPIC();
    InitPS2Mouse();
    outb(PIC1_DATA, 0b11111001);
    outb(PIC2_DATA, 0b11101111);
    asm ("sti");
}

extern "C" int main(BootData* bootdata) {
    initialize(bootdata);
    graphics->DrawRectangleFromTo(0, 0, graphics->GetWidth(), graphics->GetHeight(), 0x00FF00);

    graphics->SetColor(0x7b7b7b);
    graphics->SetXY(graphics->GetWidth()/2-_strlen("No, it's not. Please refrain from even remotely touching my pc.")*8/2, graphics->GetHeight()/2);
    graphics->printf("Is this your computer?");
    graphics->SetXY(graphics->GetWidth()/2-_strlen("No, it's not. Please refrain from even remotely touching my pc.")*8/2, graphics->GetHeight()/2+16);
    graphics->printf("No, it's not. Please refrain from even remotely touching my pc.");
    graphics->SetXY(graphics->GetWidth()/2+_strlen("No, it's not. Please refrain from even remotely touching my pc.")*8/2-_strlen("- Levi Scott Hicks")*8, graphics->GetHeight()/2+48);
    graphics->printf("- Levi Scott Hicks");
    /*
     * asm("int $0x0e");
     *  | Kernel Panic
     */
    while(true) ProcessMousePacket();
    return 300;
}