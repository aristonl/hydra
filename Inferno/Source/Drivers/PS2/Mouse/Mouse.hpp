#pragma once
#include "../../IO/IO.hpp"
#include "../../Graphics/GOP/GOP.hpp"

void PS2MouseWait() {
  unsigned long long timeout = 100000;
  while (timeout--) if ((inb(0x64) & 0b10)==0) return;
}

void PS2MouseWaitInput() {
  unsigned long long timeout = 100000;
  while (timeout--) if (inb(0x64) & 0b1) return;
}

void PS2MouseWrite(unsigned char value) {
  PS2MouseWait();
  outb(0x64, 0xD4);
  PS2MouseWait();
  outb(0x60, value);
}

unsigned char PS2MouseRead() {
  PS2MouseWaitInput();
  return inb(0x60);
}

unsigned char PS2MouseCycle = 0, PS2MousePacket[4];
bool PS2MousePacketReady = false;
unsigned int PS2MouseX = 0, PS2MouseY = 0, PS2MouseZ = 0;

void PS2MouseDriver() {
  outb(0x64, 0xA8);
  PS2MouseWait();
  outb(0x64, 0x20);
  PS2MouseWaitInput();
  unsigned char status = inb(0x60);
  status |= 0b10;
  PS2MouseWait();
  outb(0x64, 0x60);
  PS2MouseWait();
  outb(0x60, status);

  PS2MouseWrite(0xF6);
  PS2MouseRead();
  PS2MouseWrite(0xF4);
}

void PS2MouseUpdate(unsigned char data) {
  switch (PS2MouseCycle) {
    case 0:
      if (PS2MousePacketReady) break;
      if (data & 0b00001000 == 0) break;
      PS2MousePacket[0] = data;
      PS2MouseCycle++;
      break;
    case 1:
      if (PS2MousePacketReady) break;
      PS2MousePacket[1] = data;
      PS2MouseCycle++;
      break;
    case 2:
      if (PS2MousePacketReady) break;
      PS2MousePacket[2] = data;
      PS2MousePacketReady = true;
      PS2MouseCycle = 0;
      break;
  }
}

void PS2MousePacketHandler() {
  if (!PS2MousePacketReady) return;
  PS2MousePacketReady = false;
  bool xNegative, yNegative, xOverflow, yOverflow;
  if (PS2MousePacket[0] & 0b00010000) xNegative = true;
  else xNegative = false;
  if (PS2MousePacket[0] & 0b00100000) yNegative = true;
  else yNegative = false;
  if (PS2MousePacket[0] & 0b01000000) xOverflow = true;
  else xOverflow = false;
  if (PS2MousePacket[0] & 0b10000000) yOverflow = true;
  else yOverflow = false;

  if (!xNegative) {
    PS2MouseX += PS2MousePacket[1];
    if (xOverflow) PS2MouseX += 255;
  } else {
    PS2MousePacket[1] = 256 - PS2MousePacket[1];
    PS2MouseX -= PS2MousePacket[1];
    if (xOverflow) PS2MouseX -= 255;
  }
  if (!yNegative) {
    PS2MouseY -= PS2MousePacket[2];
    if (yOverflow) PS2MouseY -= 255;
  } else {
    PS2MousePacket[2] = 256 - PS2MousePacket[2];
    PS2MouseY += PS2MousePacket[2];
    if (yOverflow) PS2MouseY += 255;
  }
  if (PS2MouseX < 0) PS2MouseX = 0;
  if (PS2MouseX > framebuffer->Width) PS2MouseX = framebuffer->Width;
  if (PS2MouseY < 0) PS2MouseY = 0;
  if (PS2MouseY > framebuffer->Height) PS2MouseY = framebuffer->Height;
  *(unsigned int*)((unsigned int*)framebuffer->Address + PS2MouseX + (PS2MouseY * framebuffer->PPSL)) = 0xFFFFFFFF;
}