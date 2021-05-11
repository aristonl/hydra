#include "panic.h"
#include "../../graphics/graphics.h"
#include "../../misc/string.h"

void Panic(const char* panicMessage) {
    graphics->DrawRectangleFromTo(0, 0, graphics->GetWidth(), graphics->GetHeight(), 0xff0000);
}