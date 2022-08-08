#include "COM.hpp"

bool COM1Active = false;

void InitializeSerialDevice() {
  outb(0x3f8 + 1, 0x00);
  outb(0x3f8 + 3, 0x80);
  outb(0x3f8 + 0, 0x03);
  outb(0x3f8 + 1, 0x00);
  outb(0x3f8 + 3, 0x03);
  outb(0x3f8 + 2, 0xC7);
  outb(0x3f8 + 4, 0x0B);
  outb(0x3f8 + 4, 0x1E);
  outb(0x3f8 + 0, 0xAE);
  if (inb(0x3f8 + 0) != 0xAE) return;
  outb(0x3f8 + 4, 0x0F);
  COM1Active = true;
  kprintf("\n\nInferno: Serial Device Initialized!\n");
}

int SerialRecieveEvent() { return inb(0x3f8 + 5) & 1; }

char AwaitSerialResponse() {
  while (SerialRecieveEvent() == 0);
  return inb(0x3f8);
}

int SerialWait() { return inb(0x3f8 + 5) & 0x20; }

void kputchar(char a) {
  if (!COM1Active) return;
  while (SerialWait() == 0);
  outb(0x3f8,a);
}

void kprintf(const char* str) {
  if (!COM1Active) return;
  while (*str) {
    kputchar(*str);
    str++;
  }
}