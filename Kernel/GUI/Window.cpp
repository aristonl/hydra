#include "Window.h"
#include "../graphics.h"
#include "../string.h"

Window::Window() {}

Window::Window(const char* name, unsigned int x, unsigned int y, unsigned int width, unsigned int height, bool min, bool max) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->name = name;
    graphics->DrawBox(x-(width/2), y-(height/2), width, height, 0xFF212121, 5, 5, 5, 5);
    graphics->DrawCircle(x-(width/2)+11, y-(height/2)+10, 6, 0xFFFF0000, true);
    graphics->DrawCircle(x-(width/2)+26, y-(height/2)+10, 6, min ? 0xFFFFFF00 : 0xFF616161, true);
    graphics->DrawCircle(x-(width/2)+41, y-(height/2)+10, 6, max ? 0xFF00FF00 : 0xFF616161, true);
    graphics->DrawRectangle(x-(width/2), y-(height/2)+20, width, 1, 0xFF424242);
    graphics->SetXY(x-_strlen(name)/2*8, y-(height/2)+2);
    graphics->SetColor(0xFFFFFFFF);
    graphics->printf(name);
}

void Window::destroy() {
    graphics->DrawBox(x-(width/2), y-(height/2), width, height, 0x000000, 5, 5, 5, 5);
}