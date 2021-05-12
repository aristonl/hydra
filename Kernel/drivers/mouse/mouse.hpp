/*
 * Copyright Null LLC
 * Please read the License!
 *  _     _           _ 
 * | |   | |         | |
 * | |__ | |_   _  _ | | ____ ____
 * |  __)| | | | |/ || |/ ___) _  |     mouse.hpp
 * | |   | | |_| ( (_| | |  ( ( | |     Controls the Cursor
 * |_|   |_|\__  |\____|_|   \_||_|
 *         (____/
 */

#pragma once
#include "../../misc/InputOutput.hpp"
#include "../../graphics/graphics.hpp"

#define LeftMouseButton 0b00000001
#define MiddleMouseButton 0b00000100
#define RightMouseButton 0b00000010
#define MouseXSign 0b00010000
#define MouseYSign 0b00100000
#define MouseXOverflow 0b01000000
#define MouseYOverflow 0b10000000

class Mouse {
    public:
        Mouse();
        void HandleData(uint8_t data);
        void StepPacket();
    private:
        void MouseWait();
        void MouseWaitInput();
        void MouseWrite(uint8_t value);
        uint8_t MouseRead();
        uint8_t MouseCycle = 0;
        uint8_t MousePacket[4];
        bool MousePacketReady = false;
        Point MousePosition = {10,10};
};

extern Mouse mouse;