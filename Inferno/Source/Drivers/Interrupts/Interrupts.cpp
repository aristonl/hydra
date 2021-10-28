#include "Interrupts.hpp"
#include "../IO/IO.hpp"
#include "../../Graphics/Font/Font.hpp"
#include "../Graphics/GOP/GOP.hpp"
#include "../Memory/Memory.hpp"

size_t strlen(const char *str) {
  const char* s;
  for (s=str;*s;++s);
  return (s-str);
}

Interrupt void PageFaultHandler(struct InterruptFrame* frame) {
  memset(framebuffer->Address, 0, framebuffer->Size);
  CursorX=framebuffer->Width/2-(strlen("Page Fault!")*8/2);
  CursorY=framebuffer->Height/2-4;
  printf("Page Fault!");
  asm("hlt");
}

Interrupt void DoubleFaultHandler(struct InterruptFrame* frame) {
  memset(framebuffer->Address, 0, framebuffer->Size);
  CursorX=framebuffer->Width/2-(strlen("Double Fault!")*8/2);
  CursorY=framebuffer->Height/2-4;
  printf("Double Fault!");
  asm("hlt");
}

Interrupt void GeneralProtectionFaultHandler(struct InterruptFrame* frame) {
  memset(framebuffer->Address, 0, framebuffer->Size);
  CursorX=framebuffer->Width/2-(strlen("General Protection Fault!")*8/2);
  CursorY=framebuffer->Height/2-4;
  printf("General Protection Fault!");
  asm("hlt");
}

Interrupt void PS2KeyboardHandler(struct InterruptFrame* frame) {
  uint8_t code = inb(0x60);
  PICEndMaster();
}

void PICEndMaster() {
  outb(PIC1_COMMAND, PIC_EOI);
}

void PICEndSlave() {
  outb(PIC2_COMMAND, PIC_EOI);
  outb(PIC1_COMMAND, PIC_EOI);
}

void MapPIC() {
  uint8_t a1, a2; 
  a1 = inb(PIC1_DATA);
  io_wait();
  a2 = inb(PIC2_DATA);
  io_wait();
  outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
  io_wait();
  outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
  io_wait();
  outb(PIC1_DATA, 0x20);
  io_wait();
  outb(PIC2_DATA, 0x28);
  io_wait();
  outb(PIC1_DATA, 4);
  io_wait();
  outb(PIC2_DATA, 2);
  io_wait();
  outb(PIC1_DATA, ICW4_8086);
  io_wait();
  outb(PIC2_DATA, ICW4_8086);
  io_wait();
  outb(PIC1_DATA, a1);
  io_wait();
  outb(PIC2_DATA, a2);
}