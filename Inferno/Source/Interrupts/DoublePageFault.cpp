#include <Interrupts/DoublePageFault.hpp>
#include <COM.hpp>

void DoublePageFault(void*) {
    kprintf("\r\e[31m[ERROR] Double Page Fault\e[0m\n\r");
    asm("hlt");
}