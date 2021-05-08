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
}

extern "C" int main(BootData* bootdata) {
    initialize(bootdata);
    graphics->putpixel(bootdata->framebuffer->Width/2, bootdata->framebuffer->Height/2, 0xFFFFFF);

    graphics->printf("root@Hydra (/) > ");
    
    /*
     * asm("int $0x0e");
     *  | Kernel Panic
     */
    while(true) ProcessMousePacket();
    return 300;
}