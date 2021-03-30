#include "graphics.h"

#define PI 3.14159265

extern "C" void main(Framebuffer* framebuffer) {
    GUI gui;
    const int centerX = framebuffer->Width / 2;
    const int centerY = framebuffer->Height / 2;
    gui.SetFramebuffer(framebuffer);
    gui.DrawRectangle(0,0,framebuffer->Width,framebuffer->Height,0xFFFF0000);
    gui.DrawRectangle(centerX-50, centerY-50, 50, 50, 0xFF75CC54);
    gui.DrawCursor(400, 300, 0xFF000000, 0xFFFFFFFF);
    while (1) __asm__ ("hlt");
    //putpixel(0,0,0xFFFFFFFF, framebuffer);
    return;
}