#include "interrupts.h"
#include "../panic.h"
#include "../io.h"
#include "../string.h"
#include "keyboard.h"

__attribute__((interrupt)) void PageFault_Handler(struct interrupt_frame* frame) {
    Panic("Page Fault Detected");
    while(true);
}

__attribute__((interrupt)) void DoubleFault_Handler(struct interrupt_frame* frame) {
    Panic("Double Fault Detected");
    while(true);
}

__attribute__((interrupt)) void GPFault_Handler(struct interrupt_frame* frame) {
    Panic("General Protection Fault Detected");
    while(true);
}

__attribute__((interrupt)) void KeyboardInt_Handler(struct interrupt_frame* frame) {
    // Keypress
    uint8_t keycode = inb(0x60);
    
    gui->SetXY(0,0);
    gui->DrawRectangleFromTo(0,0,16,16,0);
    gui->printf(to_hstring((uint8_t)keycode));
    gui->SetXY(0,16);
    gui->DrawRectangleFromTo(0,16,150,32,0);
    gui->printf(getName(keycode));
    PIC_EndMaster();
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