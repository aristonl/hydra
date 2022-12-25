#include <sys/io.h>
#include <cmos.h>

#include <COM.hpp>

unsigned char second; // 0x00
unsigned char minute; // 0x02
unsigned char hour;  // 0x04

unsigned char day; // 0x07
unsigned char month; // 0x08
unsigned int year; // 0x09

int century_register = 0x00;

unsigned char registerB; // 0x0B

unsigned char getRTCRegister(int reg) {
      outb(cmos_address, reg);
      return inb(cmos_data);
}

void readRTC() {
	second = getRTCRegister(0x00);
	minute = getRTCRegister(0x02);
	hour = getRTCRegister(0x04);

	day = getRTCRegister(0x07);
	month = getRTCRegister(0x08);
	year = getRTCRegister(0x09);

	// TODO: Add 2000 to year so it'll display the full 4-digit year

	registerB = getRTCRegister(0x0B);

	// Convert BCD to binary values if necessary

	if (!(registerB & 0x04)) {
		second = (second & 0x0F) + ((second / 16) * 10);
		minute = (minute & 0x0F) + ((minute / 16) * 10);
		hour = ( (hour & 0x0F) + (((hour & 0x70) / 16) * 10) ) | (hour & 0x80);
		day = (day & 0x0F) + ((day / 16) * 10);
		month = (month & 0x0F) + ((month / 16) * 10);
		year = (year & 0x0F) + ((year / 16) * 10);
	}

	// Convert 12 hour clock to 24 hour clock if necessary

	if (!(registerB & 0x02) && (hour & 0x80)) {
		hour = ((hour & 0x7F) + 12) % 24;
	}

	kprintf("\r\e[92m[KERNEL] rtc: %u/%u/%u %u:%u:%u\e[0m\n\r", day, month, year, hour, minute, second);
}
