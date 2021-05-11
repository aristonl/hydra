/*
 * Copyright Null LLC
 * Please read the License!
 *  _     _           _ 
 * | |   | |         | |
 * | |__ | |_   _  _ | | ____ ____
 * |  __)| | | | |/ || |/ ___) _  |     mouse.hpp
 * | |   | | |_| ( (_| | |  ( ( | |     Controls the Cursor
 * |_|   |_|\__  |\____|_|   \_||_|         Will be replaced soon!
 *         (____/
 */

#pragma once
#include "../../misc/InputOutput.hpp"
#include "../../misc/math.hpp"
#include "../../graphics/graphics.hpp"

#define PS2Leftbutton 0b00000001
#define PS2Middlebutton 0b00000100
#define PS2Rightbutton 0b00000010
#define PS2XSign 0b00010000
#define PS2YSign 0b00100000
#define PS2XOverflow 0b01000000
#define PS2YOverflow 0b10000000

void InitPS2Mouse();
void HandlePS2Mouse(uint8_t data);
void ProcessMousePacket();
extern Point MousePosition;