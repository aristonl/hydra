#pragma once

namespace PIT {
  extern double TimeSinceBoot;
  const unsigned long long BaseFrequency = 1193182;
  void Sleepd(double seconds);
  void Sleep(unsigned int seconds);
  void SetDivisor(unsigned short NewDivisor);
  unsigned long long GetFrequency();
  void SetFrequency(unsigned long long NewFrequency);
  void Tick();
}