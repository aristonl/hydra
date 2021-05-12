/*
 * Copyright Null LLC
 * Please read the License!
 *  _     _           _ 
 * | |   | |         | |
 * | |__ | |_   _  _ | | ____ ____
 * |  __)| | | | |/ || |/ ___) _  |     mouse.cpp
 * | |   | | |_| ( (_| | |  ( ( | |     Controls the Cursor
 * |_|   |_|\__  |\____|_|   \_||_|
 *         (____/
 */

#include "mouse.hpp"
#include "../../misc/string.hpp"

Mouse mouse = Mouse();

void Mouse::MouseWait() {
    uint64_t timeout = 100000;
    while (timeout--) if ((inb(0x64) & 0b10) == 0) return;
}

void Mouse::MouseWaitInput() {
    uint64_t timeout = 100000;
    while (timeout--) if (inb(0x64) & 0b1) return;
}

void Mouse::MouseWrite(uint8_t value) {
    MouseWait();
    outb(0x64, 0xD4);
    MouseWait();
    outb(0x60, value);
}

uint8_t Mouse::MouseRead() {
    MouseWaitInput();
    return inb(0x60);
}

void Mouse::HandleData(uint8_t data) {
    switch(MouseCycle) {
        case 0:
            if (MousePacketReady) break;
            if (data & 0b00001000 == 0) break;
            MousePacket[0] = data;
            MouseCycle++;
            break;
        case 1:
            if (MousePacketReady) break;
            MousePacket[1] = data;
            MouseCycle++;
            break;
        case 2:
            if (MousePacketReady) break;
            MousePacket[2] = data;
            MousePacketReady = true;
            MouseCycle = 0;
            break;
    }
}

void Mouse::StepPacket() {
    Point old = MousePosition;
    if (!MousePacketReady) return;
    bool xNegative, yNegative, xOverflow, yOverflow;
    if (MousePacket[0] & MouseXSign) xNegative = true;
    else xNegative = false;
    if (MousePacket[0] & MouseYSign) yNegative = true;
    else yNegative = false;
    if (MousePacket[0] & MouseXOverflow) xOverflow = true;
    else xOverflow = false;
    if (MousePacket[0] & MouseYOverflow) yOverflow = true;
    else yOverflow = false;

    if (!xNegative) {
        MousePosition.X += MousePacket[1];
        if (xOverflow) MousePosition.X += 255;
    } else {
        MousePacket[1] = 256 - MousePacket[1];
        MousePosition.X -= MousePacket[1];
        if (xOverflow) MousePosition.X -= 255;
    }
    if (!yNegative) {
        MousePosition.Y -= MousePacket[2];
        if (yOverflow) MousePosition.Y -= 255;
    } else {
        MousePacket[2] = 256 - MousePacket[2];
        MousePosition.Y += MousePacket[2];
        if (yOverflow) MousePosition.Y += 255;
    }

    if (MousePosition.X < 0) MousePosition.X = old.X;
    if (MousePosition.X > graphics->framebuffer->Width-1) MousePosition.X = old.X;
    
    if (MousePosition.Y < 0) MousePosition.Y = old.Y;
    if (MousePosition.Y > graphics->framebuffer->Height-1) MousePosition.Y = old.Y;

    if (MousePacket[0] & LeftMouseButton) graphics->putpixel(MousePosition.X, MousePosition.Y, 0xFF0000);
    else if (MousePacket[0] & RightMouseButton) graphics->putpixel(MousePosition.X, MousePosition.Y, 0x00FF00);
    else graphics->DrawCursor(MousePosition.X, MousePosition.Y);

    MousePacketReady = false;
}

Mouse::Mouse() {
    outb(0x64, 0xA8);
    MouseWait();
    outb(0x64, 0x20);
    MouseWaitInput();
    uint8_t status = inb(0x60);
    status |= 0b10;
    MouseWait();
    outb(0x64, 0x60);
    MouseWait();
    outb(0x60, status);
    MouseWrite(0xF6);
    MouseRead();
    MouseWrite(0xF4);
    MouseRead();
}