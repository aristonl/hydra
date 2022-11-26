#pragma once

namespace APIC {
    bool Capable();
    void SetBase(unsigned int base);
    unsigned int GetBase();
    void write_register(unsigned int reg, unsigned int value);
    unsigned int read_register(unsigned int reg);
    void Enable();
}