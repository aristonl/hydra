#include "graphics.h"
#include "string.h"
#include "math.h"
#include <stdint.h>

extern "C" int main(Framebuffer* framebuffer, PSF1_FONT* font) {
    if (font == NULL) return 1;
    if (framebuffer == NULL) return 2;
    GUI gui;
    gui.SetFramebuffer(framebuffer);
    const int centerX = framebuffer->Width / 2;
    const int centerY = framebuffer->Height / 2;
    gui.SetFont(font);
    gui.DrawRectangleFromTo(0, 0, framebuffer->Width, framebuffer->Height, 0x57f542);
    gui.SetXY(centerX-72, centerY/2-8);
    gui.SetColor(0xFFFFFF);
    gui.printf("Welcome to HydraOS");
    gui.SetXY(centerX-73, centerY/2-9);
    gui.SetColor(0x000000);
    gui.printf("Welcome to HydraOS");
    gui.SwapBuffers();
    gui.DrawRectangleFromTo(0, 0, framebuffer->Width, framebuffer->Height, 0x57f542);
    gui.SwapBuffers();
    gui.DrawCursor(centerX, centerY, 0);
    while (true) __asm__("hlt");
}