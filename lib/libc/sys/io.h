#pragma once

unsigned char inb(unsigned short port);
unsigned char inb_p(unsigned short port);
unsigned short inw(unsigned short port);
unsigned short inw_p(unsigned short port);
unsigned int inl(unsigned short port);
unsigned int inl_p(unsigned short port);

void outb(unsigned short port, unsigned char value);
void outb_p(unsigned char value, unsigned short port);
void outw(unsigned short value, unsigned short port);
void outw_p(unsigned short value, unsigned short port);
void outl(unsigned short port, unsigned int value);
void outl_p(unsigned int value, unsigned short port);

void insb(unsigned short port, void* addr,
    unsigned long count);
void insw(unsigned short port, void* addr,
    unsigned long count);
void insl(unsigned short port, void* addr,
    unsigned long count);
void outsb(unsigned short port, const void* addr,
    unsigned long count);
void outsw(unsigned short port, const void* addr,
    unsigned long count);
void outsl(unsigned short port, const void* addr,
    unsigned long count);

void io_wait();
