#pragma once
#include "graphics.h"
#include "string.h"
#include "math.h"
#include "bootloader.h"
#include "memory.h"
#include "gdt.h"
#include "interrupts/idt.h"
#include "interrupts/interrupts.h"
#include "interrupts/keyboard.h"
#include "io.h"
#include "GUI/Display.h"
#include <stdint.h>

extern uint64_t KernelStart;
extern uint64_t KernelEnd;

int InitOS(BootData* bootdata);
int StartOS(BootData* bootdata);
void Test();