#ifndef IO
#define IO

void outb(unsigned short port, unsigned char value);
unsigned char inb(unsigned short port);
void io_wait();

#endif