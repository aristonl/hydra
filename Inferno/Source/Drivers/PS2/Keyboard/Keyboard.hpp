#pragma once
#ifndef PS2KeyboardDriver
#define PS2KeyboardDriver

#include "../../../Vector.h"

unsigned char PS2KeyboardMapScanCode[85] = {
  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20,
  0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40,
  0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x57, 0x58,

  // Special Keys
  0xE0
};

const char* PS2KeyboardMapFormatted[] = {
  "\e", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "\b", "\t", "q", "w", "e", "r", "t", "y",
  "u", "i", "o", "p", "[", "]", "\n", "lctrl", "a", "s", "d", "f", "g", "h", "j", "k", "l", ";", "'", "`",
  "lshift", "\\", "z", "x", "c", "v", "b", "n", "m", ",", ".", "/", "rshift", "k*", "lalt", " ", "capslock", "F1",
  "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "numlock", "scrolllock", "k7", "k8", "k9", "k-", "k4", "k5",
  "k6", "k+", "k1", "k2", "k3", "k0", "k.", "F11", "F12",


  // Special Keys
  "lsuper"
};

Vector<char*> PS2ActiveKeys;

char* GetActiveKeys() {
  return PS2ActiveKeys[0];
}

void SetKey(unsigned char key) {
  if (key >= 0x80 && key <= 0xD8) {
    key -= 0x80;
    for (unsigned int i=0;i<sizeof(PS2KeyboardMapScanCode)/sizeof(PS2KeyboardMapScanCode[0]);i++) {
      if (key == PS2KeyboardMapScanCode[i]) {
        PS2ActiveKeys.pop_back();
      }
    }
  }
  for (unsigned int i=0;i<sizeof(PS2KeyboardMapScanCode)/sizeof(PS2KeyboardMapScanCode[0]);i++) {
    if (key == PS2KeyboardMapScanCode[i]) {
      PS2ActiveKeys.push_back((char*)PS2KeyboardMapFormatted[i]);
    }
  }
}

#endif