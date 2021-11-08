#include "Drivers/Graphics/GOP/GOP.hpp"
#include "Graphics/Font/Font.hpp"
#include "Drivers/Memory/Memory.hpp"
#include "Graphics/Images/TGA/TGA.hpp"
#include "Drivers/Interrupts/GDT/GDT.hpp"
#include "Drivers/Interrupts/IDT/IDT.hpp"
#include "Drivers/Interrupts/Interrupts.hpp"
#include "Drivers/IO/IO.hpp"
#include "Drivers/ACPI/ACPI.hpp"
#include "Drivers/PCI/PCI.hpp"

extern uint64_t InfernoStart;
extern uint64_t InfernoEnd;

IDTR idtr;

__attribute__((sysv_abi)) void Inferno(Framebuffer* framebuffer, PSFFont* font, MemoryDescriptor* Map, unsigned long long int MapSize, unsigned long long int DescriptorSize, TGAImage* BootLogo, ACPI::RSDP2* rsdp) {
  SetGlobalFramebuffer(framebuffer);
  SetGlobalFont(font);
  printf("Loading Global Descriptor Table...\n");
  GDTDescriptor descriptor;
  descriptor.Size = sizeof(GDT)-1;
  descriptor.Offset = (uint64_t)&DefaultGDT;

  uint64_t MapEntries = MapSize / DescriptorSize;
  Allocator=PageFrameAllocator();
  Allocator.ReadMemoryMap(Map, MapSize, DescriptorSize);
  uint64_t kernelSize = (uint64_t)&InfernoEnd-(uint64_t)&InfernoStart;
  uint64_t kernelPages = (uint64_t)kernelSize/4096+1;
  Allocator.LockPages(&InfernoStart, kernelPages);

  printf("Loading Page Table...\n");
  PageTable* pageTable = (PageTable*)Allocator.RequestPage();
  memset(pageTable, 0, 0x1000);
  pageTableManager = PageTableManager(pageTable);
  for (uint64_t t=0;t<GetMemorySize(Map, MapEntries, DescriptorSize);t+=0x1000) pageTableManager.MapMemory((void*)t, (void*)t);
  asm ("mov %0, %%cr3" : : "r" (pageTable));
  uint64_t fbBase = (uint64_t)framebuffer->Address;
  uint64_t fbSize = (uint64_t)framebuffer->Size + 0x1000;
  Allocator.LockPages((void*)fbBase, fbSize/0x1000 + 1);
  for (uint64_t t = fbBase; t < fbBase + fbSize; t += 4096) pageTableManager.MapMemory((void*)t, (void*)t);
  InitializeHeap((void*)0x0000100000000000, 0x10);
  backbuffer = ((uint8_t*) (malloc(framebuffer->Width * framebuffer->Height *4)));
  ClearBuffer();
  SwapBuffers();
  CursorX = 0; CursorY = 0;

  LoadGDT(&descriptor);
  idtr.Limit = 0x0FFF;
  idtr.Offset = (uint64_t)Allocator.RequestPage();

  printf("Loading Page Fault interrupt...\n");
  InterruptDescriptorTableEntry* PageFault = (InterruptDescriptorTableEntry*)(idtr.Offset + 0xE * sizeof(InterruptDescriptorTableEntry));
  PageFault->SetOffset((uint64_t)PageFaultHandler);
  PageFault->type_attr = IDT_TA_InterruptGate;
  PageFault->selector = 0x08;

  printf("Loading Double Fault interrupt...\n");
  InterruptDescriptorTableEntry* DoubleFault = (InterruptDescriptorTableEntry*)(idtr.Offset + 0x8 * sizeof(InterruptDescriptorTableEntry));
  DoubleFault->SetOffset((uint64_t)DoubleFaultHandler);
  DoubleFault->type_attr = IDT_TA_InterruptGate;
  DoubleFault->selector = 0x08;

  printf("Loading General Protection Fault interrupt...\n");
  InterruptDescriptorTableEntry* GeneralProtectionFault = (InterruptDescriptorTableEntry*)(idtr.Offset + 0xD * sizeof(InterruptDescriptorTableEntry));
  GeneralProtectionFault->SetOffset((uint64_t)GeneralProtectionFaultHandler);
  GeneralProtectionFault->type_attr = IDT_TA_InterruptGate;
  GeneralProtectionFault->selector = 0x08;

  printf("Loading PS2 Keyboard interrupt...\n");
  InterruptDescriptorTableEntry* PS2Keyboard = (InterruptDescriptorTableEntry*)(idtr.Offset + 0x21 * sizeof(InterruptDescriptorTableEntry));
  PS2Keyboard->SetOffset((uint64_t)PS2KeyboardHandler);
  PS2Keyboard->type_attr = IDT_TA_InterruptGate;
  PS2Keyboard->selector = 0x08;

  printf("Loading PS2 Mouse interrupt...\n");
  InterruptDescriptorTableEntry* PS2Mouse = (InterruptDescriptorTableEntry*)(idtr.Offset + 0x2C * sizeof(InterruptDescriptorTableEntry));
  PS2Mouse->SetOffset((uint64_t)PS2MouseHandler);
  PS2Mouse->type_attr = IDT_TA_InterruptGate;
  PS2Mouse->selector = 0x08;

  asm("lidt %0" :: "m" (idtr));

  MapPIC();
  
  outb(PIC1_DATA, 0b11111001);
  outb(PIC2_DATA, 0b11101111);
  asm("sti");

  printf("Printing Bootlogo...\n");
  ClearBuffer();
  unsigned int* img = (unsigned int*)BootLogo->buffer;
  unsigned int height = BootLogo->header_ptr->height;
  unsigned int width = BootLogo->header_ptr->width;
  for (size_t dy=0;dy<height;dy++) {
    for (size_t dx=0;dx<width;dx++) {
      size_t offset = dx+(height*dy);
      unsigned int color = *(img+offset);
      size_t x = dx+(framebuffer->Width/2)-(width/2);
      size_t y = dy+(framebuffer->Height/2)-(height/2);
      putpixel(x, y, color);
    }
  }

  SwapBuffers();

  printf((Allocator.GetFreeMem()+Allocator.GetUsedMem())/1024/1024);
  printf(" MB of RAM\n");

  
  ACPI::SDTHeader* xsdt = (ACPI::SDTHeader*)(rsdp->XSDTAddress);
  ACPI::MCFGHeader* mcfg;
  if (ACPI::FindTable(xsdt, (char*)"MCFG")) {
    mcfg = (ACPI::MCFGHeader*)(ACPI::FindTable(xsdt, (char*)"MCFG"));

    for (int t=0;t<4;t++) {
      putc(mcfg->Header.Signature[t], CursorX+=8, CursorY);
    }
    printf("\n");
    PCI::EnumeratePCI(mcfg);
  } else printf("Could not locate MCFG!\n");

  printf("Inferno v0.204\n");

  while(true) asm("hlt");
}

__attribute__((ms_abi)) void main(Framebuffer* framebuffer, PSFFont* font, MemoryDescriptor* Map, unsigned long long int MapSize, unsigned long long int DescriptorSize, TGAImage* BootLogo, ACPI::RSDP2* rsdp) {
  Inferno(framebuffer, font, Map, MapSize, DescriptorSize, BootLogo, rsdp);
}