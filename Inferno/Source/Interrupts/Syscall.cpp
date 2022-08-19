#include <Interrupts/Syscall.hpp>
#include <COM.hpp>

void SyscallHandler(void*) {
    kprintf("Hello World!\n\r");
}