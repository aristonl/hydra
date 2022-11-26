#include <Interrupts/APIC.hpp>
#include <cpuid.h>

namespace APIC {
    bool Capable() {
        unsigned int eax, unused, edx;
        __get_cpuid(1, &eax, &unused, &unused, &edx);
        return edx & 1 << 9;
    }

    void SetBase(unsigned int base) {
        unsigned int edx = 0;
        unsigned int eax = (base & 0xfffff0000) | 0x800;

        #ifdef __PHYSICAL_MEMORY_EXTENSION__
            edx = (base >> 32) & 0x0f;
        #endif

        asm volatile("wrmsr" : : "a"(0x1B), "d"(eax), "c"(edx));
    }

    unsigned int GetBase() {
        unsigned int eax, edx;
        asm volatile("wrmsr" : : "a"(0x1B), "d"(eax), "c"(edx));

        #ifdef __PHYSICAL_MEMORY_EXTENSION__
            return (eax & 0xfffff000) | ((edx & 0x0f) << 32);
        #else
            return (eax & 0xfffff000);
        #endif
    }

    void write_register(unsigned int reg, unsigned int value) {
        unsigned int* apic = (unsigned int*)GetBase();
        apic[reg / 4] = value;
    }

    unsigned int read_register(unsigned int reg) {
        unsigned int* apic = (unsigned int*)GetBase();
        return apic[reg / 4];
    }

    void Enable() {
        SetBase(GetBase());
        write_register(0xF0, read_register(0xF0) | 0x100);
    }
}