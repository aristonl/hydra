#ifndef UEFI
  #define UEFI
  #include "Inferno.hpp"
  extern "C" void InfernoUEFI() {
    Inferno();
  }
#endif