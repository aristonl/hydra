#pragma once

#include <sys/io.h>

enum {
      cmos_address = 0x70,
      cmos_data    = 0x71
};

unsigned char get_RTC_register(int reg);

void readRTC();
