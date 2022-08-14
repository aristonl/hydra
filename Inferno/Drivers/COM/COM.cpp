#include "COM.hpp"

bool COM1Active = false;

void InitializeSerialDevice() {
  // Initialize COM1
  outb(0x3f8 + 1, 0x00);
  outb(0x3f8 + 3, 0x80);
  outb(0x3f8 + 0, 0x03);
  outb(0x3f8 + 1, 0x00);
  outb(0x3f8 + 3, 0x03);
  outb(0x3f8 + 2, 0xC7);
  outb(0x3f8 + 4, 0x0B);
  outb(0x3f8 + 4, 0x1E);
  outb(0x3f8 + 0, 0xAE);
  // Check if COM1 is valid
  if (inb(0x3f8 + 0) != 0xAE) return;
  outb(0x3f8 + 4, 0x0F);
  // Report active
  COM1Active = true;
  kprintf("\n\nInferno: Serial Device Initialized!\n");
}

// Check if data is being received on COM1
int SerialRecieveEvent() { return inb(0x3f8 + 5) & 1; }

// Wait for serial data to be received
char AwaitSerialResponse() {
  while (SerialRecieveEvent() == 0);
  return inb(0x3f8);
}

int SerialWait() { return inb(0x3f8 + 5) & 0x20; }

// Send a character to COM1
void kputchar(char a) {
  if (!COM1Active) return;
  while (SerialWait() == 0);
  outb(0x3f8,a);
}

// Send a string to COM1
void kprintf(const char* str) {
  if (!COM1Active) return;
  while (*str) {
    kputchar(*str);
    str++;
  }
}