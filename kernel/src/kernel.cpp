#include "graphics.h"
#include "string.h"
#include "math.h"
#include "bootloader.h"
#include <stdint.h>

extern "C" int main(BootData* bootdata) {
    if (bootdata->font == NULL) return 1;
    if (bootdata->framebuffer == NULL) return 2;
    GUI gui;
    gui.SetFramebuffer(bootdata->framebuffer);
    const int centerX = bootdata->framebuffer->Width / 2;
    const int centerY = bootdata->framebuffer->Height / 2;
    gui.SetFont(bootdata->font);
    gui.DrawRectangleFromTo(0, 0, bootdata->framebuffer->Width, bootdata->framebuffer->Height, 0x00C0FF);
    gui.SetXY(centerX-72, centerY/2-8);
    gui.SetColor(0xFFFFFF);
    gui.printf("Welcome to HydraOS");
    gui.SetXY(centerX-73, centerY/2-9);
    gui.SetColor(0x000000);
    gui.printf("Welcome to HydraOS");
    gui.DrawCursor(centerX, centerY, 0);
    while (true) __asm__("hlt");
}