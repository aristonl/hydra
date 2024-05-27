#include <TTY.h>
#include <Memory/Mem_.hpp>
#include <stdint.h>
// #define SSFN_CONSOLEBITMAP_TRUECOLOR
// #include <ssfn.h>

void* font;
Framebuffer* fb;

void SetFramebuffer(Framebuffer* _fb) {
    fb = _fb;
}

void SetFont(void* _font) {
    font = _font;
}

uint8_t* buffer;

void putpixel(unsigned int x, unsigned int y, unsigned int color) {
    buffer[(4*fb->PPSL*y+4*x)] = (color & 0xFF);
    buffer[(4*fb->PPSL*y+4*x)+1] = (color & 0xFF00) >> 8;
    buffer[(4*fb->PPSL*y+4*x)+2] = (color & 0xFF0000) >> 16;
    buffer[(4*fb->PPSL*y+4*x)+3] = (color & 0xFF000000) >> 24;
}

void swapBuffers() {
    memcpy(fb->Address, buffer, fb->Width * fb->Height * 4);
}

void test() {
    // no malloc yet so we have to use a static buffer
    buffer = (uint8_t*)0x100000; // TODO: make this dynamic
    memset(fb->Address, 0, fb->Width * fb->Height * 4);
    for (int i = 0; i < fb->Width; i++) {
        for (int j = 0; j < fb->Height; j++) {
            putpixel(i, j, 0);
        }
    }

    // ssfn_src = (ssfn_font_t*)font;      /* the bitmap font to use */
    // ssfn_dst.ptr = buffer;                  /* framebuffer address and bytes per line */
    // ssfn_dst.p = 4*fb->PPSL;
    // ssfn_dst.fg = 0xFFFFFFFF;                   /* colors, white on black */
    // ssfn_dst.bg = 0;
    // ssfn_dst.x = 100;                           /* coordinates to draw to */
    // ssfn_dst.y = 200;
    
    // /* render text directly to the screen and then adjust ssfn_dst.x and ssfn_dst.y */
    // ssfn_putc('H');
    // ssfn_putc('e');
    // ssfn_putc('l');
    // ssfn_putc('l');
    // ssfn_putc('o');

    swapBuffers();
}
