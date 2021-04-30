#include "os.h"

PageTableManager pageTableManager = NULL;

IDTR idtr;
void SetIDTGate(void* handler, uint8_t entryOffset, uint8_t selector) {
    IDTDescEntry* interrupt = (IDTDescEntry*)(idtr.Offset + entryOffset * sizeof(IDTDescEntry));
    interrupt->SetOffset((uint64_t)handler);
    interrupt->type_attr = type_attr;
    interrupt->selector = selector;
}


int InitOS(BootData* bootdata) {
    if (bootdata->font == NULL) return 1;
    if (bootdata->framebuffer == NULL) return 2;
    if (bootdata->Map == NULL) return 3;
    GDTDescriptor gdtDescriptor;
    gdtDescriptor.Size = sizeof(GDT)-1;
    gdtDescriptor.Offset = (uint64_t) &DefaultGDT;
    LoadGDT(&gdtDescriptor);
    Allocator = PageFrameAllocator();
    uint64_t MapEntries = bootdata->MapSize / bootdata->MapDescriptorSize;
    Allocator.ReadEFIMemoryMap(bootdata->Map, bootdata->MapSize, bootdata->MapDescriptorSize);
    uint64_t kernelSize = (uint64_t) &KernelEnd - (uint64_t) &KernelStart;
    uint64_t kernelPages = (uint64_t)kernelSize / 4096 + 1;
    Allocator.LockPages(&KernelStart, kernelPages);
    PageTable* PML4 = (PageTable*) Allocator.RequestPage();
    memset(PML4, 0, 0x1000);
    pageTableManager = PageTableManager(PML4);
    uint64_t fbBase = (uint64_t) bootdata->framebuffer->BaseAddress;
    uint64_t fbSize = (uint64_t) bootdata->framebuffer->Size + 0x1000;
    for (uint64_t t = 0; t < getMemorySize(bootdata->Map, MapEntries, bootdata->MapDescriptorSize); t += 0x1000) pageTableManager.MapMemory((void*)t, (void*)t);
    Allocator.LockPages((void*)fbBase, fbSize / 0x1000 + 1);
    for (uint64_t t = fbBase; t < fbBase + fbSize; t += 4096) pageTableManager.MapMemory((void*) t, (void*) t);
    asm ("mov %0, %%cr3" : : "r" (PML4));
    
    idtr.Limit = 0x0FFF;
    idtr.Offset = (uint64_t) Allocator.RequestPage();

    SetIDTGate((void*)PageFault_Handler, 0xE, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)DoubleFault_Handler, 0x8, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)GPFault_Handler, 0xD, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)KeyboardInt_Handler, 0x21, IDT_TA_InterruptGate, 0x08);

    IDT

    asm ("lidt %0" : : "m" (idtr));

    RemapPIC();

    outb(PIC1_DATA, 0b11111101);
    outb(PIC2_DATA, 0b11111111);

    asm ("sti");

    graphics->SetFramebuffer(bootdata->framebuffer);
    graphics->SetFont(bootdata->font);
    graphics->SetXY(0,0);

    memset(bootdata->framebuffer->BaseAddress, 0, bootdata->framebuffer->Size);

    return 4664;
}

int StartOS(BootData* bootdata) {
    graphics->printf("root@Hydra (/) > ");

    // asm("int $0x0e");            Force Panic
    
    while (true) asm("hlt");
    return 300;
}
