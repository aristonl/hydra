#include "Manager.h"

Manager::Manager() {}

void Manager::ShowWindow() {
    Window("Manager", graphics->GetWidth()/2, graphics->GetHeight()/2, 800, 400, false, false);
}

void Manager::HideWindow() {
    Window("Manager", graphics->GetWidth()/2, graphics->GetHeight()/2, 800, 400, false, false).destroy();
}