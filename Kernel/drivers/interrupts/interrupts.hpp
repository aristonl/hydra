/*
 * Copyright Null LLC
 * Please read the License!
 *  _     _           _ 
 * | |   | |         | |
 * | |__ | |_   _  _ | | ____ ____
 * |  __)| | | | |/ || |/ ___) _  |     interrupts.hpp
 * | |   | | |_| ( (_| | |  ( ( | |     Controls kernel panics, PS2 Keyboards, mouses, etc...
 * |_|   |_|\__  |\____|_|   \_||_|
 *         (____/
 */

#pragma once
#include "../../graphics/graphics.hpp"

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1
#define PIC_EOI 0x20

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

struct interrupt_frame;
__attribute__((interrupt)) void PageFaultHandler(interrupt_frame* frame);
__attribute__((interrupt)) void DoublePageFaultHandler(interrupt_frame* frame);
__attribute__((interrupt)) void GeneralPageFaultHandler(interrupt_frame* frame);
__attribute__((interrupt)) void KeyboardHandler(interrupt_frame* frame);
__attribute__((interrupt)) void MouseHandler(interrupt_frame* frame);

void RemapPIC();
void PIC_EndMaster();
void PIC_EndSlave();
