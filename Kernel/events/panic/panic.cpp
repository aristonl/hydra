#include "panic.h"
#include "../../graphics/graphics.h"
#include "../../misc/string.h"

void Panic(const char* panicMessage) {
    graphics->DrawRectangleFromTo(0, 0, graphics->GetWidth(), graphics->GetHeight(), 0xff0000);

    unsigned int centerX = graphics->GetWidth()/2;
    unsigned int centerY = graphics->GetHeight()/2;

    graphics->SetXY(centerX-_strlen("Kernel Panic")*8/2, centerY-8);

    graphics->SetColor(0xFFFFFF);
    graphics->printf("Kernel Panic");
    graphics->SetXY(centerX-_strlen(panicMessage)*8/2, centerY-8+32);

    graphics->printf(panicMessage);
}