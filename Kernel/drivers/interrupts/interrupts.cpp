#include "interrupts.h"
#include "../../misc/InputOutput.h"
#include "../../misc/string.h"
#include "mouse.h"

__attribute__((interrupt)) void PageFault_Handler(interrupt_frame* frame) {
    graphics->DrawRectangleFromTo(0, 0, graphics->GetWidth(), graphics->GetHeight(), 0xff0000);
    while(true);
}

__attribute__((interrupt)) void DoubleFault_Handler(interrupt_frame* frame) {
    graphics->DrawRectangleFromTo(0, 0, graphics->GetWidth(), graphics->GetHeight(), 0xff0000);
    while(true);
}

__attribute__((interrupt)) void GPFault_Handler(interrupt_frame* frame) {
    graphics->DrawRectangleFromTo(0, 0, graphics->GetWidth(), graphics->GetHeight(), 0xff0000);
    while(true);
}

__attribute__((interrupt)) void KeyboardInt_Handler(interrupt_frame* frame) {
    uint8_t keycode = inb(0x60);
    
    graphics->DrawRectangleFromTo(128,128,160,144,0);
    graphics->SetXY(128, 128);
    graphics->printf("0x");
    graphics->printf(to_hstring(keycode));
    PIC_EndMaster();
}

__attribute__((interrupt)) void MouseInt_Handler(interrupt_frame* frame) {
    uint8_t mouseData = inb(0x60);
    HandlePS2Mouse(mouseData);
    PIC_EndSlave();
}

void PIC_EndMaster() {
    outb(PIC1_COMMAND, PIC_EOI);
}

void PIC_EndSlave() {
    outb(PIC2_COMMAND, PIC_EOI);
    outb(PIC1_COMMAND, PIC_EOI);
}
   

void RemapPIC() {
    uint8_t a1, a2; 
    a1 = inb(PIC1_DATA);
    io_wait();
    a2 = inb(PIC2_DATA);
    io_wait();

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    outb(PIC1_DATA, 0x20);
    io_wait();
    outb(PIC2_DATA, 0x28);
    io_wait();

    outb(PIC1_DATA, 4);
    io_wait();
    outb(PIC2_DATA, 2);
    io_wait();

    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    outb(PIC1_DATA, a1);
    io_wait();
    outb(PIC2_DATA, a2);
}