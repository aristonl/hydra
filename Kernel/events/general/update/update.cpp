/*
 * Copyright Null LLC
 * Please read the License!
 *  _     _           _ 
 * | |   | |         | |
 * | |__ | |_   _  _ | | ____ ____
 * |  __)| | | | |/ || |/ ___) _  |     update.cpp
 * | |   | | |_| ( (_| | |  ( ( | |     An event that occurs when nothing else is happening.
 * |_|   |_|\__  |\____|_|   \_||_|
 *         (____/
 */

#include "update.hpp"
#include "../../../drivers/interrupts/mouse.hpp"
#include "../../../memory/memory.hpp"
#include "../../../graphics/graphics.hpp"

void UpdateEvent() {
    ProcessMousePacket();
}