#include "panic.h"
#include "graphics.h"
#include "string.h"

void Panic(const char* panicMessage) {
    gui->DrawRectangleFromTo(0, 0, gui->GetWidth(), gui->GetHeight(), 0xff0000);

    unsigned int centerX = gui->GetWidth()/2;
    unsigned int centerY = gui->GetHeight()/2;

    gui->SetXY(centerX-_strlen("Kernel Panic")*8/2, centerY-8);

    gui->SetColor(0xFFFFFF);
    gui->printf("Kernel Panic");
    gui->SetXY(centerX-_strlen(panicMessage)*8/2, centerY-8+32);

    gui->printf(panicMessage);
}