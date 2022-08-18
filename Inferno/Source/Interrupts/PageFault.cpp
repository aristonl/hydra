#include <Interrupts/PageFault.hpp>
#include <COM.hpp>

void PageFault(void*) {
    kprintf("\r\e[31m[ERROR] Page Fault\e[0m\n\r");
    asm("hlt");
}