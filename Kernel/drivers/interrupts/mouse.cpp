/*
 * Copyright Null LLC
 * Please read the License!
 *  _     _           _ 
 * | |   | |         | |
 * | |__ | |_   _  _ | | ____ ____
 * |  __)| | | | |/ || |/ ___) _  |     mouse.cpp
 * | |   | | |_| ( (_| | |  ( ( | |     Controls the Cursor
 * |_|   |_|\__  |\____|_|   \_||_|         Will be replaced soon!
 *         (____/
 */

#include "mouse.hpp"
#include "../../misc/string.hpp"

void MouseWait() {
    uint64_t timeout = 100000;
    while (timeout--) if ((inb(0x64) & 0b10) == 0) return;
}

void MouseWaitInput() {
    uint64_t timeout = 100000;
    while (timeout--) if (inb(0x64) & 0b1) return;
}

void MouseWrite(uint8_t value) {
    MouseWait();
    outb(0x64, 0xD4);
    MouseWait();
    outb(0x60, value);
}

uint8_t MouseRead() {
    MouseWaitInput();
    return inb(0x60);
}

uint8_t MouseCycle = 0;
uint8_t MousePacket[4];
bool MousePacketReady = false;
Point MousePosition;
void HandlePS2Mouse(uint8_t data) {
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

void ProcessMousePacket() {
    Point old = MousePosition;
    if (!MousePacketReady) return;
    bool xNegative, yNegative, xOverflow, yOverflow;
    if (MousePacket[0] & PS2XSign) xNegative = true;
    else xNegative = false;
    if (MousePacket[0] & PS2YSign) yNegative = true;
    else yNegative = false;
    if (MousePacket[0] & PS2XOverflow) xOverflow = true;
    else xOverflow = false;
    if (MousePacket[0] & PS2YOverflow) yOverflow = true;
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
    if (MousePosition.X > graphics->GetWidth()-12) MousePosition.X = old.X;
    
    if (MousePosition.Y < 0) MousePosition.Y = old.Y;
    if (MousePosition.Y > graphics->GetHeight()-19) MousePosition.Y = old.Y;


    if (MousePacket[0] & PS2Leftbutton) {
        graphics->putpixel(MousePosition.X, MousePosition.Y, 0xFF0000);
    } else if (MousePacket[0] & PS2Rightbutton) {
        graphics->putpixel(MousePosition.X, MousePosition.Y, 0x00FF00);
    }

    MousePacketReady = false;
}

void InitPS2Mouse() {
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
    graphics->DrawCursor(MousePosition.X, MousePosition.Y, 0);
}