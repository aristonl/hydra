/*
 * Copyright Null LLC
 * Please read the License!
 *  _     _           _ 
 * | |   | |         | |
 * | |__ | |_   _  _ | | ____ ____
 * |  __)| | | | |/ || |/ ___) _  |     interrupts.cpp
 * | |   | | |_| ( (_| | |  ( ( | |     Controls kernel panics, PS2 Keyboards, mouses, etc...
 * |_|   |_|\__  |\____|_|   \_||_|
 *         (____/
 */

#include "interrupts.hpp"
#include "../../misc/InputOutput.hpp"
#include "../../misc/string.hpp"
#include "../mouse/mouse.hpp"

__attribute__((interrupt)) void PageFaultHandler(interrupt_frame* frame) {
    graphics->DrawRectangleFromTo(0, 0, graphics->framebuffer->Width, graphics->framebuffer->Height, 0x0000ff);
    graphics->color = 16777215;
    graphics->TextPosition.X = graphics->framebuffer->Width/2-_strlen("Page Fault")*8/2;
    graphics->TextPosition.Y = graphics->framebuffer->Height/2-48;
    graphics->printf("Page Fault");
    graphics->TextPosition.X = graphics->framebuffer->Width/2-_strlen("Hydra encountered an error whilst paging!")*8/2;
    graphics->TextPosition.Y = graphics->framebuffer->Height/2-16;
    graphics->printf("Hydra encountered an error whilst paging!");
    graphics->TextPosition.X = graphics->framebuffer->Width-_strlen("Error code: 0x0000")*8;
    graphics->TextPosition.Y = graphics->framebuffer->Height-16;
    graphics->printf("Error code: 0x000");
    while(true);
}

__attribute__((interrupt)) void DoublePageFaultHandler(interrupt_frame* frame) {
    graphics->DrawRectangleFromTo(0, 0, graphics->framebuffer->Width, graphics->framebuffer->Height, 0x0000ff);
    graphics->color = 16777215;
    graphics->TextPosition.X = graphics->framebuffer->Width/2-_strlen("Double Page Fault")*8/2;
    graphics->TextPosition.Y = graphics->framebuffer->Height/2-48;
    graphics->printf("Double Page Fault");
    graphics->TextPosition.X = graphics->framebuffer->Width/2-_strlen("Hydra encountered an error whilst paging!")*8/2;
    graphics->TextPosition.Y = graphics->framebuffer->Height/2-16;
    graphics->printf("Hydra encountered an error whilst paging!");
    graphics->TextPosition.X = graphics->framebuffer->Width-_strlen("Error code: 0x0001")*8;
    graphics->TextPosition.Y = graphics->framebuffer->Height-16;
    graphics->printf("Error code: 0x0001");
    while(true);
}

__attribute__((interrupt)) void GeneralPageFaultHandler(interrupt_frame* frame) {
    graphics->DrawRectangleFromTo(0, 0, graphics->framebuffer->Width, graphics->framebuffer->Height, 0x0000ff);
    graphics->color = 16777215;
    graphics->TextPosition.X = graphics->framebuffer->Width/2-_strlen("General Page Fault")*8/2;
    graphics->TextPosition.Y = graphics->framebuffer->Height/2-48;
    graphics->printf("General Page Fault");
    graphics->TextPosition.X = graphics->framebuffer->Width/2-_strlen("Hydra encountered an error whilst paging!")*8/2;
    graphics->TextPosition.Y = graphics->framebuffer->Height/2-16;
    graphics->printf("Hydra encountered an error whilst paging!");
    graphics->TextPosition.X = graphics->framebuffer->Width-_strlen("Error code: 0x0002")*8;
    graphics->TextPosition.Y = graphics->framebuffer->Height-16;
    graphics->printf("Error code: 0x0002");
    while(true);
}

__attribute__((interrupt)) void KeyboardHandler(interrupt_frame* frame) {
    uint8_t keycode = inb(0x60);
    graphics->DrawRectangleFromTo(128,128,160,144,0);
    graphics->TextPosition = {128, 128};
    graphics->printf("0x");
    graphics->printf(to_hstring(keycode));
    PIC_EndMaster();
}

__attribute__((interrupt)) void MouseHandler(interrupt_frame* frame) {
    uint8_t mouseData = inb(0x60);
    mouse.HandleData(mouseData);
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