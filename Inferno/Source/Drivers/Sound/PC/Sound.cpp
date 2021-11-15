#include "Sound.hpp"
#include "../../IO/IO.hpp"

void PlaySound(unsigned long freq) {
  unsigned int Div;
  unsigned char tmp;

  Div = 1193180 / freq;
  outb(0x43, 0xb6);
 	outb(0x42, (uint8_t)(Div));
 	outb(0x42, (uint8_t)(Div >> 8));

  tmp = inb(0x61);
  if (tmp != (tmp | 3)) {
    outb(0x61, tmp | 3);
  }
}

void StopSound() {
  unsigned char tmp = inb(0x61) & 0xFC;
  outb(0x61, tmp);
}