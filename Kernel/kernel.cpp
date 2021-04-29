#include "os.h"

extern "C" int main(BootData* bootdata) {
    int result = InitOS(bootdata);
    if (result != 4664) return result;
    return StartOS(bootdata);
    return 0;
}