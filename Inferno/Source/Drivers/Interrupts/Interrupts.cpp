#include "Interrupts.hpp"
#include "../../Graphics/Font/Font.hpp"

__attribute__((interrupt)) void PageFaultHandler(struct InterruptFrame* frame) {
  printf("Page Fault!");
  asm("hlt");
}