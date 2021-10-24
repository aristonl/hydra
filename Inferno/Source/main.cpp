#include "Drivers/Graphics/GOP/GOP.hpp"
#include "Graphics/Font/Font.hpp"
#include "Drivers/Memory/Memory.hpp"
#include "Graphics/Images/TGA/TGA.hpp"
#include "Drivers/Interrupts/GDT/GDT.hpp"
#include "Drivers/Interrupts/IDT/IDT.hpp"
#include "Drivers/Interrupts/Interrupts.hpp"
#include "Drivers/IO/IO.hpp"

extern uint64_t InfernoStart;
extern uint64_t InfernoEnd;

void putpixel(unsigned int x, unsigned int y, unsigned int color) {
  *(unsigned int*)((unsigned int*)framebuffer->Address + x + (y * framebuffer->PPSL)) = 0;
}

IDTR idtr;

__attribute__((sysv_abi)) void main(Framebuffer* framebuffer, PSFFont* font, Memory* memory, TGAImage* BootLogo) {
  SetGlobalFramebuffer(framebuffer);
  SetGlobalFont(font);
  GDTDescriptor descriptor;
  descriptor.Size = sizeof(GDT)-1;
  descriptor.Offset = (uint64_t)&DefaultGDT;

  uint64_t MapEntries = memory->MapSize / memory->DescriptorSize;
  Allocator=PageFrameAllocator();
  Allocator.ReadMemoryMap(memory->Map, memory->MapSize, memory->DescriptorSize);
  uint64_t kernelSize = (uint64_t)&InfernoEnd-(uint64_t)&InfernoStart;
  uint64_t kernelPages = (uint64_t)kernelSize/4096+1;
  Allocator.LockPages(&InfernoStart, kernelPages);

  LoadGDT(&descriptor);
  idtr.Limit = 0x0FFF;
  idtr.Offset = (uint64_t)Allocator.RequestPage();

  IDTDescriptorEntry* int_PageFault = (IDTDescriptorEntry*)(idtr.Offset + 0xE * sizeof(IDTDescriptorEntry));
  int_PageFault->SetOffset((uint64_t)PageFaultHandler);
  int_PageFault->type_attr = IDT_TA_InterruptGate;
  int_PageFault->selector = 0x08;

  IDTDescriptorEntry* int_DoubleFault = (IDTDescriptorEntry*)(idtr.Offset + 0x8 * sizeof(IDTDescriptorEntry));
  int_DoubleFault->SetOffset((uint64_t)DoubleFaultHandler);
  int_DoubleFault->type_attr = IDT_TA_InterruptGate;
  int_DoubleFault->selector = 0x08;

  IDTDescriptorEntry* int_GPFault = (IDTDescriptorEntry*)(idtr.Offset + 0xD * sizeof(IDTDescriptorEntry));
  int_GPFault->SetOffset((uint64_t)GeneralProtectionFaultHandler);
  int_GPFault->type_attr = IDT_TA_InterruptGate;
  int_GPFault->selector = 0x08;

  IDTDescriptorEntry* int_Keyboard = (IDTDescriptorEntry*)(idtr.Offset + 0x21 * sizeof(IDTDescriptorEntry));
  int_Keyboard->SetOffset((uint64_t)PS2KBHandler);
  int_Keyboard->type_attr = IDT_TA_InterruptGate;
  int_Keyboard->selector = 0x08;

  asm("lidt %0" :: "m" (idtr));

  MapPIC();
  outb(PIC1_DATA, 0b11111101);
  outb(PIC2_DATA, 0b11111111);
  asm("sti");

  PageTable* pageTable = (PageTable*)Allocator.RequestPage();
  memset(pageTable, 0, 0x1000);
  PageTableManager pageTableManager = PageTableManager(pageTable);
  for (uint64_t t=0;t<GetMemorySize(memory->Map, MapEntries, memory->DescriptorSize);t+=0x1000) pageTableManager.MapMemory((void*)t, (void*)t);
  memset(framebuffer->Address, 0, framebuffer->Size);

  unsigned int* img = (unsigned int*)BootLogo->buffer;
  unsigned int height = BootLogo->header_ptr->height;
  unsigned int width = BootLogo->header_ptr->width;
  for (size_t dy=0;dy<height;dy++) {
    for (size_t dx=0;dx<width;dx++) {
      size_t offset = dx+(height*dy);
      unsigned int color = *(img+offset);
      size_t x = dx+(framebuffer->Width/2)-(width/2);
      size_t y = dy+(framebuffer->Height/2)-(height/2);
      *(unsigned int*)((unsigned int*)framebuffer->Address+x+(y*framebuffer->PPSL)) = color;
    }
  }
  
  uint64_t framebufferAddress = (uint64_t)framebuffer->Address;
  uint64_t framebufferSize = (uint64_t)framebuffer->Size+0x1000;
  Allocator.LockPages((void*)framebufferAddress, framebufferSize/0x1000+1);
  for (uint64_t t=framebufferAddress;t<framebufferAddress+framebufferSize;t+=0x1000) pageTableManager.MapMemory((void*)t, (void*)t);
  asm("mov %0, %%cr3" :: "r" (pageTable));

  printf("Echo v0.174\n");

  // int* test = (int*)0x800000000;
  // *test = 2;
  
  while(true);
}