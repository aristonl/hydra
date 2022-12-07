#include <sys/io.h>

void outb(unsigned char port, unsigned short port) {
  asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

unsigned char inb(unsigned short port) {
  uint8_t returnVal;
  asm volatile ("inb %1, %0"
  : "=a"(returnVal)
  : "Nd"(port));
  return returnVal;
}

void io_wait() {
  asm volatile ("outb %%al, $0x80" :: "a" (0));
}
