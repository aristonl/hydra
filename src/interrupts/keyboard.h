#pragma once

char* getInfo(uint8_t keycode, char *keychar, char **keystr) {
    *keychar = '\0';
    *keystr = (char*) "";
    bool isKeyPressed = false;
    switch (keycode) {
        case 0x01:
            *keychar = '\0';
            *keystr = (char*) "ESC";
            isKeyPressed = true;
            break;
        case 0x81:
            *keychar = '\0';
            *keystr = (char*) "ESC";
            isKeyPressed = false;
            break;
        case 0x02:
            *keychar = '1';
            *keystr = (char*) "1";
            isKeyPressed = true;
            break;
        case 0x82:
            *keychar = '1';
            *keystr = (char*) "1";
            isKeyPressed = false;
            break;
        case 0x03:
            *keychar = '2';
            *keystr = (char*) "2";
            isKeyPressed = true;
            break;
        case 0x83:
            *keychar = '2';
            *keystr = (char*) "2";
            isKeyPressed = false;
            break;
        case 0x04:
            *keychar = '3';
            *keystr = (char*) "3";
            isKeyPressed = true;
            break;
        case 0x84:
            *keychar = '3';
            *keystr = (char*) "3";
            isKeyPressed = false;
            break;
        case 0x05:
            *keychar = '4';
            *keystr = (char*) "4";
            isKeyPressed = true;
            break;
        case 0x85:
            *keychar = '4';
            *keystr = (char*) "4";
            isKeyPressed = false;
            break;
        case 0x06:
            *keychar = '5';
            *keystr = (char*) "5";
            isKeyPressed = true;
            break;
        case 0x86:
            *keychar = '5';
            *keystr = (char*) "5";
            isKeyPressed = false;
            break;
        case 0x07:
            *keychar = '6';
            *keystr = (char*) "6";
            isKeyPressed = true;
            break;
        case 0x87:
            *keychar = '6';
            *keystr = (char*) "6";
            isKeyPressed = false;
            break;
        case 0x08:
            *keychar = '7';
            *keystr = (char*) "7";
            isKeyPressed = true;
            break;
        case 0x88:
            *keychar = '7';
            *keystr = (char*) "7";
            isKeyPressed = false;
            break;
        case 0x09:
            *keychar = '8';
            *keystr = (char*) "8";
            isKeyPressed = true;
            break;
        case 0x89:
            *keychar = '8';
            *keystr = (char*) "8";
            isKeyPressed = false;
            break;
        case 0x0A:
            *keychar = '9';
            *keystr = (char*) "9";
            isKeyPressed = true;
            break;
        case 0x8A:
            *keychar = '9';
            *keystr = (char*) "9";
            isKeyPressed = false;
            break;
        case 0x0B:
            *keychar = '0';
            *keystr = (char*) "0";
            isKeyPressed = true;
            break;
        case 0x8B:
            *keychar = '0';
            *keystr = (char*) "0";
            isKeyPressed = false;
            break;
        case 0x0C:
            *keychar = '-';
            *keystr = (char*) "Dash";
            isKeyPressed = true;
            break;
        case 0x8C:
            *keychar = '-';
            *keystr = (char*) "Dash";
            isKeyPressed = false;
            break;
        case 0x0D:
            *keychar = '=';
            *keystr = (char*) "Equals";
            isKeyPressed = true;
            break;
        case 0x8D:
            *keychar = '=';
            *keystr = (char*) "Equals";
            isKeyPressed = false;
            break;
        case 0x0E:
            *keychar = '\0';
            *keystr = (char*) "Backspace";
            isKeyPressed = true;
            break;
        case 0x8E:
            *keychar = '\0';
            *keystr = (char*) "Backspace";
            isKeyPressed = false;
            break;
        case 0x0F:
            *keychar = '\t';
            *keystr = (char*) "Tab";
            isKeyPressed = true;
            break;
        case 0x8F:
            *keychar = '\t';
            *keystr = (char*) "Tab";
            isKeyPressed = false;
            break;
        case 0x10:
            *keychar = 'Q';
            *keystr = (char*) "Q";
            isKeyPressed = true;
            break;
        case 0x90:
            *keychar = 'Q';
            *keystr = (char*) "Q";
            isKeyPressed = false;
            break;
        case 0x11:
            *keychar = 'W';
            *keystr = (char*) "W";
            isKeyPressed = true;
            break;
        case 0x91:
            *keychar = 'W';
            *keystr = (char*) "W";
            isKeyPressed = false;
            break;
        case 0x12:
            *keychar = 'E';
            *keystr = (char*) "E";
            isKeyPressed = true;
            break;
        case 0x92:
            *keychar = 'E';
            *keystr = (char*) "E";
            isKeyPressed = false;
            break;
        case 0x13:
            *keychar = 'R';
            *keystr = (char*) "R";
            isKeyPressed = true;
            break;
        case 0x93:
            *keychar = 'R';
            *keystr = (char*) "R";
            isKeyPressed = false;
            break;
        case 0x14:
            *keychar = 'T';
            *keystr = (char*) "T";
            isKeyPressed = true;
            break;
        case 0x94:
            *keychar = 'T';
            *keystr = (char*) "T";
            isKeyPressed = false;
            break;
        case 0x15:
            *keychar = 'Y';
            *keystr = (char*) "Y";
            isKeyPressed = true;
            break;
        case 0x95:
            *keychar = 'Y';
            *keystr = (char*) "Y";
            isKeyPressed = false;
            break;
        case 0x16:
            *keychar = 'U';
            *keystr = (char*) "U";
            isKeyPressed = true;
            break;
        case 0x96:
            *keychar = 'U';
            *keystr = (char*) "U";
            isKeyPressed = false;
            break;
        case 0x17:
            *keychar = 'I';
            *keystr = (char*) "I";
            isKeyPressed = true;
            break;
        case 0x97:
            *keychar = 'I';
            *keystr = (char*) "I";
            isKeyPressed = false;
            break;
        case 0x18:
            *keychar = 'O';
            *keystr = (char*) "O";
            isKeyPressed = true;
            break;
        case 0x98:
            *keychar = 'O';
            *keystr = (char*) "O";
            isKeyPressed = false;
            break;
        case 0x19:
            *keychar = 'P';
            *keystr = (char*) "P";
            isKeyPressed = true;
            break;
        case 0x99:
            *keychar = 'P';
            *keystr = (char*) "P";
            isKeyPressed = false;
            break;
        case 0x1A:
            *keychar = '[';
            *keystr = (char*) "Left Bracket";
            isKeyPressed = true;
            break;
        case 0x9A:
            *keychar = '[';
            *keystr = (char*) "Left Bracket";
            isKeyPressed = false;
            break;
        case 0x1B:
            *keychar = ']';
            *keystr = (char*) "Right Bracket";
            isKeyPressed = true;
            break;
        case 0x9B:
            *keychar = ']';
            *keystr = (char*) "Right Bracket";
            isKeyPressed = false;
            break;
        case 0x1C:
            *keychar = '\n';
            *keystr = (char*) "Enter";
            isKeyPressed = true;
            break;
        case 0x9C:
            *keychar = '\n';
            *keystr = (char*) "Enter";
            isKeyPressed = false;
            break;
        case 0x1D:
            *keychar = '\0';
            *keystr = (char*) "Left Control";
            isKeyPressed = true;
            break;
        case 0x9D:
            *keychar = '\0';
            *keystr = (char*) "Left Control";
            isKeyPressed = false;
            break;
        case 0x1E:
            *keychar = 'A';
            *keystr = (char*) "A";
            isKeyPressed = true;
            break;
        case 0x9E:
            *keychar = 'A';
            *keystr = (char*) "A";
            isKeyPressed = false;
            break;
        case 0x1F:
            *keychar = 'S';
            *keystr = (char*) "S";
            isKeyPressed = true;
            break;
        case 0x9F:
            *keychar = 'S';
            *keystr = (char*) "S";
            isKeyPressed = false;
            break;
        case 0x20:
            *keychar = 'D';
            *keystr = (char*) "D";
            isKeyPressed = true;
            break;
        case 0xA0:
            *keychar = 'D';
            *keystr = (char*) "D";
            isKeyPressed = false;
            break;
        case 0x21:
            *keychar = 'F';
            *keystr = (char*) "F";
            isKeyPressed = true;
            break;
        case 0xA1:
            *keychar = 'F';
            *keystr = (char*) "F";
            isKeyPressed = false;
            break;
        case 0x22:
            *keychar = 'G';
            *keystr = (char*) "G";
            isKeyPressed = true;
            break;
        case 0xA2:
            *keychar = 'G';
            *keystr = (char*) "G";
            isKeyPressed = false;
            break;
        case 0x23:
            *keychar = 'H';
            *keystr = (char*) "H";
            isKeyPressed = true;
            break;
        case 0xA3:
            *keychar = 'H';
            *keystr = (char*) "H";
            isKeyPressed = false;
            break;
        case 0x24:
            *keychar = 'J';
            *keystr = (char*) "J";
            isKeyPressed = true;
            break;
        case 0xA4:
            *keychar = 'J';
            *keystr = (char*) "J";
            isKeyPressed = false;
            break;
        case 0x25:
            *keychar = 'K';
            *keystr = (char*) "K";
            isKeyPressed = true;
            break;
        case 0xA5:
            *keychar = 'K';
            *keystr = (char*) "K";
            isKeyPressed = false;
            break;
        case 0x26:
            *keychar = 'L';
            *keystr = (char*) "L";
            isKeyPressed = true;
            break;
        case 0xA6:
            *keychar = 'L';
            *keystr = (char*) "L";
            isKeyPressed = false;
            break;
        case 0x27:
            *keychar = ';';
            *keystr = (char*) "Semicolon";
            isKeyPressed = true;
            break;
        case 0xA7:
            *keychar = ';';
            *keystr = (char*) "Semicolon";
            isKeyPressed = false;
            break;
        case 0x28:
            *keychar = '\'';
            *keystr = (char*) "Single Quote";
            isKeyPressed = true;
            break;
        case 0xA8:
            *keychar = '\'';
            *keystr = (char*) "Single Quote";
            isKeyPressed = false;
            break;
        case 0x29:
            *keychar = '`';
            *keystr = (char*) "Grave";
            isKeyPressed = true;
            break;
        case 0xA9:
            *keychar = '`';
            *keystr = (char*) "Grave";
            isKeyPressed = false;
            break;
        case 0x2A:
            *keychar = '\0';
            *keystr = (char*) "Left Shift";
            isKeyPressed = true;
            break;
        case 0xAA:
            *keychar = '\0';
            *keystr = (char*) "Left Shift";
            isKeyPressed = false;
            break;
        case 0x2B:
            *keychar = '\\';
            *keystr = (char*) "Backslash";
            isKeyPressed = true;
            break;
        case 0xAB:
            *keychar = '\\';
            *keystr = (char*) "Backslash";
            isKeyPressed = false;
            break;
        case 0x2C:
            *keychar = 'Z';
            *keystr = (char*) "Z";
            isKeyPressed = true;
            break;
        case 0xAC:
            *keychar = 'Z';
            *keystr = (char*) "Z";
            isKeyPressed = false;
            break;
        case 0x2D:
            *keychar = 'X';
            *keystr = (char*) "X";
            isKeyPressed = true;
            break;
        case 0xAD:
            *keychar = 'X';
            *keystr = (char*) "X";
            isKeyPressed = false;
            break;
        case 0x2E:
            *keychar = 'C';
            *keystr = (char*) "C";
            isKeyPressed = true;
            break;
        case 0xAE:
            *keychar = 'C';
            *keystr = (char*) "C";
            isKeyPressed = false;
            break;
        case 0x2F:
            *keychar = 'V';
            *keystr = (char*) "V";
            isKeyPressed = true;
            break;
        case 0xAF:
            *keychar = 'V';
            *keystr = (char*) "V";
            isKeyPressed = false;
            break;
        case 0x30:
            *keychar = 'B';
            *keystr = (char*) "B";
            isKeyPressed = true;
            break;
        case 0xB0:
            *keychar = 'B';
            *keystr = (char*) "B";
            isKeyPressed = false;
            break;
        case 0x31:
            *keychar = 'N';
            *keystr = (char*) "N";
            isKeyPressed = true;
            break;
        case 0xB1:
            *keychar = 'N';
            *keystr = (char*) "N";
            isKeyPressed = false;
            break;
        case 0x32:
            *keychar = 'M';
            *keystr = (char*) "M";
            isKeyPressed = true;
            break;
        case 0xB2:
            *keychar = 'M';
            *keystr = (char*) "M";
            isKeyPressed = false;
            break;
        case 0x33:
            *keychar = ',';
            *keystr = (char*) "Comma";
            isKeyPressed = true;
            break;
        case 0xB3:
            *keychar = ',';
            *keystr = (char*) "Comma";
            isKeyPressed = false;
            break;
        case 0x34:
            *keychar = '.';
            *keystr = (char*) "Period";
            isKeyPressed = true;
            break;
        case 0xB4:
            *keychar = '.';
            *keystr = (char*) "Period";
            isKeyPressed = false;
            break;
        case 0x35:
            *keychar = '/';
            *keystr = (char*) "Slash";
            isKeyPressed = true;
            break;
        case 0xB5:
            *keychar = '/';
            *keystr = (char*) "Slash";
            isKeyPressed = false;
            break;
        case 0x36:
            *keychar = '\0';
            *keystr = (char*) "Right Shift";
            isKeyPressed = true;
            break;
        case 0xB6:
            *keychar = '\0';
            *keystr = (char*) "Right Shift";
            isKeyPressed = false;
            break;
        case 0x37:
            *keychar = '*';
            *keystr = (char*) "Keypad *";
            isKeyPressed = true;
            break;
        case 0xB7:
            *keychar = '*';
            *keystr = (char*) "Keypad *";
            isKeyPressed = false;
            break;
        case 0x38:
            *keychar = '\0';
            *keystr = (char*) "Left Alt";
            isKeyPressed = true;
            break;
        case 0xB8:
            *keychar = '\0';
            *keystr = (char*) "Left Alt";
            isKeyPressed = false;
            break;
        case 0x39:
            *keychar = ' ';
            *keystr = (char*) "Spacebar";
            isKeyPressed = true;
            break;
        case 0xB9:
            *keychar = ' ';
            *keystr = (char*) "Spacebar";
            isKeyPressed = false;
            break;
        case 0x3A:
            *keychar = '\0';
            *keystr = (char*) "Caps Lock";
            isKeyPressed = true;
            break;
        case 0xBA:
            *keychar = '\0';
            *keystr = (char*) "Caps Lock";
            isKeyPressed = false;
            break;
        case 0x3B:
            *keychar = '\0';
            *keystr = (char*) "F1";
            isKeyPressed = true;
            break;
        case 0xBB:
            *keychar = '\0';
            *keystr = (char*) "F1";
            isKeyPressed = false;
            break;
        case 0x3C:
            *keychar = '\0';
            *keystr = (char*) "F2";
            isKeyPressed = true;
            break;
        case 0xBC:
            *keychar = '\0';
            *keystr = (char*) "F2";
            isKeyPressed = false;
            break;
        case 0x3D:
            *keychar = '\0';
            *keystr = (char*) "F3";
            isKeyPressed = true;
            break;
        case 0xBD:
            *keychar = '\0';
            *keystr = (char*) "F3";
            isKeyPressed = false;
            break;
        case 0x3E:
            *keychar = '\0';
            *keystr = (char*) "F4";
            isKeyPressed = true;
            break;
        case 0xBE:
            *keychar = '\0';
            *keystr = (char*) "F4";
            isKeyPressed = false;
            break;
        case 0x3F:
            *keychar = '\0';
            *keystr = (char*) "F5";
            isKeyPressed = true;
            break;
        case 0xBF:
            *keychar = '\0';
            *keystr = (char*) "F5";
            isKeyPressed = false;
            break;
        case 0x40:
            *keychar = '\0';
            *keystr = (char*) "F6";
            isKeyPressed = true;
            break;
        case 0xC0:
            *keychar = '\0';
            *keystr = (char*) "F6";
            isKeyPressed = false;
            break;
        case 0x41:
            *keychar = '\0';
            *keystr = (char*) "F7";
            isKeyPressed = true;
            break;
        case 0xC1:
            *keychar = '\0';
            *keystr = (char*) "F7";
            isKeyPressed = false;
            break;
        case 0x42:
            *keychar = '\0';
            *keystr = (char*) "F8";
            isKeyPressed = true;
            break;
        case 0xC2:
            *keychar = '\0';
            *keystr = (char*) "F8";
            isKeyPressed = false;
            break;
        case 0x43:
            *keychar = '\0';
            *keystr = (char*) "F9";
            isKeyPressed = true;
            break;
        case 0xC3:
            *keychar = '\0';
            *keystr = (char*) "F9";
            isKeyPressed = false;
            break;
        case 0x44:
            *keychar = '\0';
            *keystr = (char*) "F10";
            isKeyPressed = true;
            break;
        case 0xC4:
            *keychar = '\0';
            *keystr = (char*) "F10";
            isKeyPressed = false;
            break;
        case 0x45:
            *keychar = '\0';
            *keystr = (char*) "Num Lock";
            isKeyPressed = true;
            break;
        case 0xC5:
            *keychar = '\0';
            *keystr = (char*) "Num Lock";
            isKeyPressed = false;
            break;
        case 0x46:
            *keychar = '\0';
            *keystr = (char*) "Scroll Lock";
            isKeyPressed = true;
            break;
        case 0xC6:
            *keychar = '\0';
            *keystr = (char*) "Scroll Lock";
            isKeyPressed = false;
            break;
        case 0x47:
            *keychar = '7';
            *keystr = (char*) "Keypad 7";
            isKeyPressed = true;
            break;
        case 0xC7:
            *keychar = '7';
            *keystr = (char*) "Keypad 7";
            isKeyPressed = false;
            break;
        case 0x48:
            *keychar = '8';
            *keystr = (char*) "Keypad 8";
            isKeyPressed = true;
            break;
        case 0xC8:
            *keychar = '8';
            *keystr = (char*) "Keypad 8";
            isKeyPressed = false;
            break;
        case 0x49:
            *keychar = '9';
            *keystr = (char*) "Keypad 9";
            isKeyPressed = true;
            break;
        case 0xC9:
            *keychar = '9';
            *keystr = (char*) "Keypad 9";
            isKeyPressed = false;
            break;
        case 0x4A:
            *keychar = '-';
            *keystr = (char*) "Keypad -";
            isKeyPressed = true;
            break;
        case 0xCA:
            *keychar = '-';
            *keystr = (char*) "Keypad -";
            isKeyPressed = false;
            break;
        case 0x4B:
            *keychar = '4';
            *keystr = (char*) "Keypad 4";
            isKeyPressed = true;
            break;
        case 0xCB:
            *keychar = '4';
            *keystr = (char*) "Keypad 4";
            isKeyPressed = false;
            break;
        case 0x4C:
            *keychar = '5';
            *keystr = (char*) "Keypad 5";
            isKeyPressed = true;
            break;
        case 0xCC:
            *keychar = '5';
            *keystr = (char*) "Keypad 5";
            isKeyPressed = false;
            break;
        case 0x4D:
            *keychar = '6';
            *keystr = (char*) "Keypad 6";
            isKeyPressed = true;
            break;
        case 0xCD:
            *keychar = '6';
            *keystr = (char*) "Keypad 6";
            isKeyPressed = false;
            break;
        case 0x4E:
            *keychar = '+';
            *keystr = (char*) "Keypad +";
            isKeyPressed = true;
            break;
        case 0xCE:
            *keychar = '+';
            *keystr = (char*) "Keypad +";
            isKeyPressed = false;
            break;
        case 0x4F:
            *keychar = '1';
            *keystr = (char*) "Keypad 1";
            isKeyPressed = true;
            break;
        case 0xCF:
            *keychar = '1';
            *keystr = (char*) "Keypad 1";
            isKeyPressed = false;
            break;
        case 0x50:
            *keychar = '2';
            *keystr = (char*) "Keypad 2";
            isKeyPressed = true;
            break;
        case 0xD0:
            *keychar = '2';
            *keystr = (char*) "Keypad 2";
            isKeyPressed = false;
            break;
        case 0x51:
            *keychar = '3';
            *keystr = (char*) "Keypad 3";
            isKeyPressed = true;
            break;
        case 0xD1:
            *keychar = '3';
            *keystr = (char*) "Keypad 3";
            isKeyPressed = false;
            break;
        case 0x52:
            *keychar = '0';
            *keystr = (char*) "Keypad 0";
            isKeyPressed = true;
            break;
        case 0xD2:
            *keychar = '0';
            *keystr = (char*) "Keypad 0";
            isKeyPressed = false;
            break;
        case 0x53:
            *keychar = '.';
            *keystr = (char*) "Keypad .";
            isKeyPressed = true;
            break;
        case 0xD3:
            *keychar = '.';
            *keystr = (char*) "Keypad .";
            isKeyPressed = false;
            break;
        case 0x55:
            *keychar = '\0';
            *keystr = (char*) "F11";
            isKeyPressed = true;
            break;
        case 0xD5:
            *keychar = '\0';
            *keystr = (char*) "F11";
            isKeyPressed = false;
            break;
        case 0x56:
            *keychar = '\0';
            *keystr = (char*) "Unlabeled";
            isKeyPressed = true;
            break;
        case 0xD6:
            *keychar = '\0';
            *keystr = (char*) "Unlabeled";
            isKeyPressed = false;
            break;
        case 0x57:
            *keychar = '\0';
            *keystr = (char*) "F11";
            isKeyPressed = true;
            break;
        case 0xD7:
            *keychar = '\0';
            *keystr = (char*) "F11";
            isKeyPressed = false;
            break;
        case 0x58:
            *keychar = '\0';
            *keystr = (char*) "F12";
            isKeyPressed = true;
            break;
        case 0xD8:
            *keychar = '\0';
            *keystr = (char*) "F12";
            isKeyPressed = false;
            break;
        default:
            *keychar = '\0';
            *keystr = (char*) "";
            isKeyPressed = false;
            break;
    }
    return *keystr;
}