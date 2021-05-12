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
#include "../../../drivers/mouse/mouse.hpp"

void UpdateEvent() {
    mouse.StepPacket();
}