#include <Syscall.hpp>

void kprintf(const char* str) {
	unsigned int res, func = 0x04, descriptor = 0x01;
	asm volatile("int $0x80" : "=a"(res) : "a"(func), "d"(descriptor), "D"(str): "rcx", "r11", "memory");
}