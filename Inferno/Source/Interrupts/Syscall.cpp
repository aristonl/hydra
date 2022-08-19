#include <Interrupts/Syscall.hpp>
#include <COM.hpp>

void SyscallHandler(void*) {
	unsigned int rax, rbx;
    const char* rcx;
    __asm__("": "=a"(rax), "=b"(rbx), "=c"(rcx));
    if (rax == 0x04) {
        // stdout
        if (rbx == 0x01) {
            kprintf((const char*)rcx);
        }
    }
}