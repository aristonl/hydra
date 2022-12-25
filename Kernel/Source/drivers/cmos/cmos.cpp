#include <sys/io.h>
#include <drivers/cmos/cmos.h>

unsigned char getRegister(int reg) {
      outb(cmosAddress, reg);
      return inb(cmosData);
}
