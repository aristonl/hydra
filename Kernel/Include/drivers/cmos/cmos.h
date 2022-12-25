#pragma once

#include <sys/io.h>

enum {
      cmosAddress = 0x70,
      cmosData    = 0x71
};

unsigned char getRegister(int reg);
