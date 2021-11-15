#include "PIT.hpp"
#include "../IO/IO.hpp"

namespace PIT {
  double TimeSinceBoot = 0;
  unsigned short Divisor = 65535;
  void Sleepd(double seconds) {
    double startTime = TimeSinceBoot;
    while (TimeSinceBoot < startTime + seconds) {
      asm("hlt");
    }
  }
  void Sleep(unsigned int seconds) {
    Sleepd((double)seconds/1000);
  }
  void SetDivisor(unsigned short NewDivisor) {
    if (NewDivisor < 100) NewDivisor = 100;
    Divisor = NewDivisor;
    outb(0x40, (unsigned char)(NewDivisor&0x00FF));
    io_wait();
    outb(0x40, (unsigned char)((NewDivisor&0xFF00)>>8));
  }
  unsigned long long GetFrequency() { return BaseFrequency / Divisor; }
  void SetFrequency(unsigned long long NewFrequency) { SetDivisor(BaseFrequency / NewFrequency); }
  void Tick() { TimeSinceBoot += 1 / (double) GetFrequency(); }
}