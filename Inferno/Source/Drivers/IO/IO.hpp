#ifndef IO
#define IO
#include "../../../../Libraries/LibC/stdint.h"

void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);
void io_wait();

#endif