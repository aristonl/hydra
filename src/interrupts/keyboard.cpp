#include "../graphics.h"
#include "keyboard.h"
#include "../string.h"

bool shifted = false;
bool ctrl = false;

bool keys[175] = { false, false, false, false, false,
                false, false, false, false, false,
                false, false, false, false, false,
                false, false, false, false, false,
                false, false, false, false, false,
                false, false, false, false, false,
                false, false, false, false, false,
                false, false, false, false, false,
                false, false, false, false, false,
                false, false, false, false, false,
                false, false, false, false, false,
                false, false, false, false, false,
                false, false, false, false, false,
                false, false, false, false, false,
                false, false, false, false, false,
                false, false, false, false, false,
                false, false, false, false, false,
                false, false, false, false, false,
                false, false, false, false, false,
                false, false, false, false, false,
                false, false, false, false, false,
                false, false, false, false, false,
                false, false, false, false, false,
                false, false, false, false, false,
                false, false, false, false, false,
                false, false, false, false, false,
                false, false, false, false, false,
                false, false, false, false, false,
                false, false, false, false, false,
                false, false, false, false, false,
                false, false, false, false, false,
                false, false, false, false, false,
                false, false, false, false, false,
                false, false, false, false, false,
                false, false, false, false, false};

char* getInfo(uint8_t keycode, char *keychar, char **keystr, bool *isKeyPressed) {
    *keychar = '\0';
    *keystr = (char*) "";
    *isKeyPressed = false;
    switch (keycode) {
        case 0x01:
            *keychar = '\0';
            *keystr = (char*) "ESC";
            *isKeyPressed = true;
            keys[0] = true;
            break;
        case 0x81:
            *keychar = '\0';
            *keystr = (char*) "ESC";
            *isKeyPressed = false;
            keys[0] = false;
            break;
        case 0x02:
            *keychar = '1';
            *keystr = (char*) "1";
            *isKeyPressed = true;
            keys[1] = true;
            break;
        case 0x82:
            *keychar = '1';
            *keystr = (char*) "1";
            *isKeyPressed = false;
            keys[1] = false;
            break;
        case 0x03:
            *keychar = '2';
            *keystr = (char*) "2";
            *isKeyPressed = true;
            keys[2] = true;
            break;
        case 0x83:
            *keychar = '2';
            *keystr = (char*) "2";
            *isKeyPressed = false;
            keys[2] = false;
            break;
        case 0x04:
            *keychar = '3';
            *keystr = (char*) "3";
            *isKeyPressed = true;
            keys[3] = true;
            break;
        case 0x84:
            *keychar = '3';
            *keystr = (char*) "3";
            *isKeyPressed = false;
            keys[3] = false;
            break;
        case 0x05:
            *keychar = '4';
            *keystr = (char*) "4";
            *isKeyPressed = true;
            keys[4] = true;
            break;
        case 0x85:
            *keychar = '4';
            *keystr = (char*) "4";
            *isKeyPressed = false;
            keys[4] = false;
            break;
        case 0x06:
            *keychar = '5';
            *keystr = (char*) "5";
            *isKeyPressed = true;
            keys[5] = true;
            break;
        case 0x86:
            *keychar = '5';
            *keystr = (char*) "5";
            *isKeyPressed = false;
            keys[5] = false;
            break;
        case 0x07:
            *keychar = '6';
            *keystr = (char*) "6";
            *isKeyPressed = true;
            keys[6] = true;
            break;
        case 0x87:
            *keychar = '6';
            *keystr = (char*) "6";
            *isKeyPressed = false;
            keys[6] = false;
            break;
        case 0x08:
            *keychar = '7';
            *keystr = (char*) "7";
            *isKeyPressed = true;
            keys[7] = true;
            break;
        case 0x88:
            *keychar = '7';
            *keystr = (char*) "7";
            *isKeyPressed = false;
            keys[7] = false;
            break;
        case 0x09:
            *keychar = '8';
            *keystr = (char*) "8";
            *isKeyPressed = true;
            keys[8] = true;
            break;
        case 0x89:
            *keychar = '8';
            *keystr = (char*) "8";
            *isKeyPressed = false;
            keys[8] = false;
            break;
        case 0x0A:
            *keychar = '9';
            *keystr = (char*) "9";
            *isKeyPressed = true;
            keys[9] = true;
            break;
        case 0x8A:
            *keychar = '9';
            *keystr = (char*) "9";
            *isKeyPressed = false;
            keys[9] = false;
            break;
        case 0x0B:
            *keychar = '0';
            *keystr = (char*) "0";
            *isKeyPressed = true;
            keys[10] = true;
            break;
        case 0x8B:
            *keychar = '0';
            *keystr = (char*) "0";
            *isKeyPressed = false;
            keys[10] = false;
            break;
        case 0x0C:
            *keychar = '-';
            *keystr = (char*) "Dash";
            *isKeyPressed = true;
            keys[11] = true;
            break;
        case 0x8C:
            *keychar = '-';
            *keystr = (char*) "Dash";
            *isKeyPressed = false;
            keys[11] = false;
            break;
        case 0x0D:
            *keychar = '=';
            *keystr = (char*) "Equals";
            *isKeyPressed = true;
            keys[12] = true;
            break;
        case 0x8D:
            *keychar = '=';
            *keystr = (char*) "Equals";
            *isKeyPressed = false;
            keys[12] = false;
            break;
        case 0x0E:
            *keychar = '\0';
            *keystr = (char*) "Backspace";
            *isKeyPressed = true;
            keys[12] = true;
            break;
        case 0x8E:
            *keychar = '\0';
            *keystr = (char*) "Backspace";
            *isKeyPressed = false;
            keys[12] = false;
            break;
        case 0x0F:
            *keychar = '\t';
            *keystr = (char*) "Tab";
            *isKeyPressed = true;
            keys[13] = true;
            break;
        case 0x8F:
            *keychar = '\t';
            *keystr = (char*) "Tab";
            *isKeyPressed = false;
            keys[13] = false;
            break;
        case 0x10:
            *keychar = 'Q';
            *keystr = (char*) "Q";
            *isKeyPressed = true;
            keys[14] = true;
            break;
        case 0x90:
            *keychar = 'Q';
            *keystr = (char*) "Q";
            *isKeyPressed = false;
            keys[14] = false;
            break;
        case 0x11:
            *keychar = 'W';
            *keystr = (char*) "W";
            *isKeyPressed = true;
            keys[15] = true;
            break;
        case 0x91:
            *keychar = 'W';
            *keystr = (char*) "W";
            *isKeyPressed = false;
            keys[15] = false;
            break;
        case 0x12:
            *keychar = 'E';
            *keystr = (char*) "E";
            *isKeyPressed = true;
            keys[16] = true;
            break;
        case 0x92:
            *keychar = 'E';
            *keystr = (char*) "E";
            *isKeyPressed = false;
            keys[16] = false;
            break;
        case 0x13:
            *keychar = 'R';
            *keystr = (char*) "R";
            *isKeyPressed = true;
            keys[17] = true;
            break;
        case 0x93:
            *keychar = 'R';
            *keystr = (char*) "R";
            *isKeyPressed = false;
            keys[17] = false;
            break;
        case 0x14:
            *keychar = 'T';
            *keystr = (char*) "T";
            *isKeyPressed = true;
            keys[18] = true;
            break;
        case 0x94:
            *keychar = 'T';
            *keystr = (char*) "T";
            *isKeyPressed = false;
            keys[18] = false;
            break;
        case 0x15:
            *keychar = 'Y';
            *keystr = (char*) "Y";
            *isKeyPressed = true;
            keys[19] = true;
            break;
        case 0x95:
            *keychar = 'Y';
            *keystr = (char*) "Y";
            *isKeyPressed = false;
            keys[19] = false;
            break;
        case 0x16:
            *keychar = 'U';
            *keystr = (char*) "U";
            *isKeyPressed = true;
            keys[20] = true;
            break;
        case 0x96:
            *keychar = 'U';
            *keystr = (char*) "U";
            *isKeyPressed = false;
            keys[20] = false;
            break;
        case 0x17:
            *keychar = 'I';
            *keystr = (char*) "I";
            *isKeyPressed = true;
            keys[21] = true;
            break;
        case 0x97:
            *keychar = 'I';
            *keystr = (char*) "I";
            *isKeyPressed = false;
            keys[21] = false;
            break;
        case 0x18:
            *keychar = 'O';
            *keystr = (char*) "O";
            *isKeyPressed = true;
            keys[22] = true;
            break;
        case 0x98:
            *keychar = 'O';
            *keystr = (char*) "O";
            *isKeyPressed = false;
            keys[22] = false;
            break;
        case 0x19:
            *keychar = 'P';
            *keystr = (char*) "P";
            *isKeyPressed = true;
            keys[23] = true;
            break;
        case 0x99:
            *keychar = 'P';
            *keystr = (char*) "P";
            *isKeyPressed = false;
            keys[23] = false;
            break;
        case 0x1A:
            *keychar = '[';
            *keystr = (char*) "Left Bracket";
            *isKeyPressed = true;
            keys[24] = true;
            break;
        case 0x9A:
            *keychar = '[';
            *keystr = (char*) "Left Bracket";
            *isKeyPressed = false;
            keys[24] = false;
            break;
        case 0x1B:
            *keychar = ']';
            *keystr = (char*) "Right Bracket";
            *isKeyPressed = true;
            keys[25] = true;
            break;
        case 0x9B:
            *keychar = ']';
            *keystr = (char*) "Right Bracket";
            *isKeyPressed = false;
            keys[25] = false;
            break;
        case 0x1C:
            *keychar = '\n';
            *keystr = (char*) "Enter";
            *isKeyPressed = true;
            keys[26] = true;
            break;
        case 0x9C:
            *keychar = '\n';
            *keystr = (char*) "Enter";
            *isKeyPressed = false;
            keys[26] = false;
            break;
        case 0x1D:
            *keychar = '\0';
            *keystr = (char*) "Left Control";
            *isKeyPressed = true;
            keys[27] = true;
            break;
        case 0x9D:
            *keychar = '\0';
            *keystr = (char*) "Left Control";
            *isKeyPressed = false;
            keys[27] = false;
            break;
        case 0x1E:
            *keychar = 'A';
            *keystr = (char*) "A";
            *isKeyPressed = true;
            keys[28] = true;
            break;
        case 0x9E:
            *keychar = 'A';
            *keystr = (char*) "A";
            *isKeyPressed = false;
            keys[28] = false;
            break;
        case 0x1F:
            *keychar = 'S';
            *keystr = (char*) "S";
            *isKeyPressed = true;
            keys[29] = true;
            break;
        case 0x9F:
            *keychar = 'S';
            *keystr = (char*) "S";
            *isKeyPressed = false;
            keys[29] = false;
            break;
        case 0x20:
            *keychar = 'D';
            *keystr = (char*) "D";
            *isKeyPressed = true;
            keys[30] = true;
            break;
        case 0xA0:
            *keychar = 'D';
            *keystr = (char*) "D";
            *isKeyPressed = false;
            keys[30] = false;
            break;
        case 0x21:
            *keychar = 'F';
            *keystr = (char*) "F";
            *isKeyPressed = true;
            keys[31] = true;
            break;
        case 0xA1:
            *keychar = 'F';
            *keystr = (char*) "F";
            *isKeyPressed = false;
            keys[31] = false;
            break;
        case 0x22:
            *keychar = 'G';
            *keystr = (char*) "G";
            *isKeyPressed = true;
            keys[32] = true;
            break;
        case 0xA2:
            *keychar = 'G';
            *keystr = (char*) "G";
            *isKeyPressed = false;
            keys[32] = false;
            break;
        case 0x23:
            *keychar = 'H';
            *keystr = (char*) "H";
            *isKeyPressed = true;
            keys[33] = true;
            break;
        case 0xA3:
            *keychar = 'H';
            *keystr = (char*) "H";
            *isKeyPressed = false;
            keys[33] = false;
            break;
        case 0x24:
            *keychar = 'J';
            *keystr = (char*) "J";
            *isKeyPressed = true;
            keys[34] = true;
            break;
        case 0xA4:
            *keychar = 'J';
            *keystr = (char*) "J";
            *isKeyPressed = false;
            keys[34] = false;
            break;
        case 0x25:
            *keychar = 'K';
            *keystr = (char*) "K";
            *isKeyPressed = true;
            keys[35] = true;
            break;
        case 0xA5:
            *keychar = 'K';
            *keystr = (char*) "K";
            *isKeyPressed = false;
            keys[35] = false;
            break;
        case 0x26:
            *keychar = 'L';
            *keystr = (char*) "L";
            *isKeyPressed = true;
            keys[36] = true;
            break;
        case 0xA6:
            *keychar = 'L';
            *keystr = (char*) "L";
            *isKeyPressed = false;
            keys[36] = false;
            break;
        case 0x27:
            *keychar = ';';
            *keystr = (char*) "Semicolon";
            *isKeyPressed = true;
            keys[37] = true;
            break;
        case 0xA7:
            *keychar = ';';
            *keystr = (char*) "Semicolon";
            *isKeyPressed = false;
            keys[37] = false;
            break;
        case 0x28:
            *keychar = '\'';
            *keystr = (char*) "Single Quote";
            *isKeyPressed = true;
            keys[38] = true;
            break;
        case 0xA8:
            *keychar = '\'';
            *keystr = (char*) "Single Quote";
            *isKeyPressed = false;
            keys[38] = false;
            break;
        case 0x29:
            *keychar = '`';
            *keystr = (char*) "Grave";
            *isKeyPressed = true;
            keys[39] = true;
            break;
        case 0xA9:
            *keychar = '`';
            *keystr = (char*) "Grave";
            *isKeyPressed = false;
            keys[39] = false;
            break;
        case 0x2A:
            *keychar = '\0';
            *keystr = (char*) "Left Shift";
            *isKeyPressed = true;
            keys[40] = true;
            break;
        case 0xAA:
            *keychar = '\0';
            *keystr = (char*) "Left Shift";
            *isKeyPressed = false;
            keys[40] = false;
            break;
        case 0x2B:
            *keychar = '\\';
            *keystr = (char*) "Backslash";
            *isKeyPressed = true;
            keys[41] = true;
            break;
        case 0xAB:
            *keychar = '\\';
            *keystr = (char*) "Backslash";
            *isKeyPressed = false;
            keys[41] = false;
            break;
        case 0x2C:
            *keychar = 'Z';
            *keystr = (char*) "Z";
            *isKeyPressed = true;
            keys[42] = true;
            break;
        case 0xAC:
            *keychar = 'Z';
            *keystr = (char*) "Z";
            *isKeyPressed = false;
            keys[42] = false;
            break;
        case 0x2D:
            *keychar = 'X';
            *keystr = (char*) "X";
            *isKeyPressed = true;
            keys[43] = true;
            break;
        case 0xAD:
            *keychar = 'X';
            *keystr = (char*) "X";
            *isKeyPressed = false;
            keys[43] = false;
            break;
        case 0x2E:
            *keychar = 'C';
            *keystr = (char*) "C";
            *isKeyPressed = true;
            keys[44] = true;
            break;
        case 0xAE:
            *keychar = 'C';
            *keystr = (char*) "C";
            *isKeyPressed = false;
            keys[44] = false;
            break;
        case 0x2F:
            *keychar = 'V';
            *keystr = (char*) "V";
            *isKeyPressed = true;
            keys[45] = true;
            break;
        case 0xAF:
            *keychar = 'V';
            *keystr = (char*) "V";
            *isKeyPressed = false;
            keys[45] = false;
            break;
        case 0x30:
            *keychar = 'B';
            *keystr = (char*) "B";
            *isKeyPressed = true;
            keys[46] = true;
            break;
        case 0xB0:
            *keychar = 'B';
            *keystr = (char*) "B";
            *isKeyPressed = false;
            keys[46] = false;
            break;
        case 0x31:
            *keychar = 'N';
            *keystr = (char*) "N";
            *isKeyPressed = true;
            keys[47] = true;
            break;
        case 0xB1:
            *keychar = 'N';
            *keystr = (char*) "N";
            *isKeyPressed = false;
            keys[47] = false;
            break;
        case 0x32:
            *keychar = 'M';
            *keystr = (char*) "M";
            *isKeyPressed = true;
            keys[48] = true;
            break;
        case 0xB2:
            *keychar = 'M';
            *keystr = (char*) "M";
            *isKeyPressed = false;
            keys[48] = false;
            break;
        case 0x33:
            *keychar = ',';
            *keystr = (char*) "Comma";
            *isKeyPressed = true;
            keys[49] = true;
            break;
        case 0xB3:
            *keychar = ',';
            *keystr = (char*) "Comma";
            *isKeyPressed = false;
            keys[49] = false;
            break;
        case 0x34:
            *keychar = '.';
            *keystr = (char*) "Period";
            *isKeyPressed = true;
            keys[50] = true;
            break;
        case 0xB4:
            *keychar = '.';
            *keystr = (char*) "Period";
            *isKeyPressed = false;
            keys[50] = false;
            break;
        case 0x35:
            *keychar = '/';
            *keystr = (char*) "Slash";
            *isKeyPressed = true;
            keys[51] = true;
            break;
        case 0xB5:
            *keychar = '/';
            *keystr = (char*) "Slash";
            *isKeyPressed = false;
            keys[51] = false;
            break;
        case 0x36:
            *keychar = '\0';
            *keystr = (char*) "Right Shift";
            *isKeyPressed = true;
            keys[52] = true;
            break;
        case 0xB6:
            *keychar = '\0';
            *keystr = (char*) "Right Shift";
            *isKeyPressed = false;
            keys[52] = false;
            break;
        case 0x37:
            *keychar = '*';
            *keystr = (char*) "Keypad *";
            *isKeyPressed = true;
            keys[53] = true;
            break;
        case 0xB7:
            *keychar = '*';
            *keystr = (char*) "Keypad *";
            *isKeyPressed = false;
            keys[53] = false;
            break;
        case 0x38:
            *keychar = '\0';
            *keystr = (char*) "Left Alt";
            *isKeyPressed = true;
            keys[54] = true;
            break;
        case 0xB8:
            *keychar = '\0';
            *keystr = (char*) "Left Alt";
            *isKeyPressed = false;
            keys[54] = false;
            break;
        case 0x39:
            *keychar = ' ';
            *keystr = (char*) "Spacebar";
            *isKeyPressed = true;
            keys[55] = true;
            break;
        case 0xB9:
            *keychar = ' ';
            *keystr = (char*) "Spacebar";
            *isKeyPressed = false;
            keys[55] = false;
            break;
        case 0x3A:
            *keychar = '\0';
            *keystr = (char*) "Caps Lock";
            *isKeyPressed = true;
            keys[56] = true;
            break;
        case 0xBA:
            *keychar = '\0';
            *keystr = (char*) "Caps Lock";
            *isKeyPressed = false;
            keys[56] = false;
            break;
        case 0x3B:
            *keychar = '\0';
            *keystr = (char*) "F1";
            *isKeyPressed = true;
            keys[57] = true;
            break;
        case 0xBB:
            *keychar = '\0';
            *keystr = (char*) "F1";
            *isKeyPressed = false;
            keys[57] = false;
            break;
        case 0x3C:
            *keychar = '\0';
            *keystr = (char*) "F2";
            *isKeyPressed = true;
            keys[58] = true;
            break;
        case 0xBC:
            *keychar = '\0';
            *keystr = (char*) "F2";
            *isKeyPressed = false;
            keys[58] = false;
            break;
        case 0x3D:
            *keychar = '\0';
            *keystr = (char*) "F3";
            *isKeyPressed = true;
            keys[59] = true;
            break;
        case 0xBD:
            *keychar = '\0';
            *keystr = (char*) "F3";
            *isKeyPressed = false;
            keys[59] = false;
            break;
        case 0x3E:
            *keychar = '\0';
            *keystr = (char*) "F4";
            *isKeyPressed = true;
            keys[60] = true;
            break;
        case 0xBE:
            *keychar = '\0';
            *keystr = (char*) "F4";
            *isKeyPressed = false;
            keys[60] = false;
            break;
        case 0x3F:
            *keychar = '\0';
            *keystr = (char*) "F5";
            *isKeyPressed = true;
            keys[61] = true;
            break;
        case 0xBF:
            *keychar = '\0';
            *keystr = (char*) "F5";
            *isKeyPressed = false;
            keys[61] = false;
            break;
        case 0x40:
            *keychar = '\0';
            *keystr = (char*) "F6";
            *isKeyPressed = true;
            keys[62] = true;
            break;
        case 0xC0:
            *keychar = '\0';
            *keystr = (char*) "F6";
            *isKeyPressed = false;
            keys[62] = false;
            break;
        case 0x41:
            *keychar = '\0';
            *keystr = (char*) "F7";
            *isKeyPressed = true;
            keys[63] = true;
            break;
        case 0xC1:
            *keychar = '\0';
            *keystr = (char*) "F7";
            *isKeyPressed = false;
            keys[63] = false;
            break;
        case 0x42:
            *keychar = '\0';
            *keystr = (char*) "F8";
            *isKeyPressed = true;
            keys[64] = true;
            break;
        case 0xC2:
            *keychar = '\0';
            *keystr = (char*) "F8";
            *isKeyPressed = false;
            keys[64] = false;
            break;
        case 0x43:
            *keychar = '\0';
            *keystr = (char*) "F9";
            *isKeyPressed = true;
            keys[65] = true;
            break;
        case 0xC3:
            *keychar = '\0';
            *keystr = (char*) "F9";
            *isKeyPressed = false;
            keys[65] = false;
            break;
        case 0x44:
            *keychar = '\0';
            *keystr = (char*) "F10";
            *isKeyPressed = true;
            keys[66] = true;
            break;
        case 0xC4:
            *keychar = '\0';
            *keystr = (char*) "F10";
            *isKeyPressed = false;
            keys[66] = false;
            break;
        case 0x45:
            *keychar = '\0';
            *keystr = (char*) "Num Lock";
            *isKeyPressed = true;
            keys[67] = true;
            break;
        case 0xC5:
            *keychar = '\0';
            *keystr = (char*) "Num Lock";
            *isKeyPressed = false;
            keys[67] = false;
            break;
        case 0x46:
            *keychar = '\0';
            *keystr = (char*) "Scroll Lock";
            *isKeyPressed = true;
            keys[68] = true;
            break;
        case 0xC6:
            *keychar = '\0';
            *keystr = (char*) "Scroll Lock";
            *isKeyPressed = false;
            keys[68] = false;
            break;
        case 0x47:
            *keychar = '7';
            *keystr = (char*) "Keypad 7";
            *isKeyPressed = true;
            keys[69] = true;
            break;
        case 0xC7:
            *keychar = '7';
            *keystr = (char*) "Keypad 7";
            *isKeyPressed = false;
            keys[69] = false;
            break;
        case 0x48:
            *keychar = '8';
            *keystr = (char*) "Keypad 8";
            *isKeyPressed = true;
            keys[70] = true;
            break;
        case 0xC8:
            *keychar = '8';
            *keystr = (char*) "Keypad 8";
            *isKeyPressed = false;
            keys[70] = false;
            break;
        case 0x49:
            *keychar = '9';
            *keystr = (char*) "Keypad 9";
            *isKeyPressed = true;
            keys[71] = true;
            break;
        case 0xC9:
            *keychar = '9';
            *keystr = (char*) "Keypad 9";
            *isKeyPressed = false;
            keys[71] = false;
            break;
        case 0x4A:
            *keychar = '-';
            *keystr = (char*) "Keypad -";
            *isKeyPressed = true;
            keys[72] = true;
            break;
        case 0xCA:
            *keychar = '-';
            *keystr = (char*) "Keypad -";
            *isKeyPressed = false;
            keys[72] = false;
            break;
        case 0x4B:
            *keychar = '4';
            *keystr = (char*) "Keypad 4";
            *isKeyPressed = true;
            keys[73] = true;
            break;
        case 0xCB:
            *keychar = '4';
            *keystr = (char*) "Keypad 4";
            *isKeyPressed = false;
            keys[73] = false;
            break;
        case 0x4C:
            *keychar = '5';
            *keystr = (char*) "Keypad 5";
            *isKeyPressed = true;
            keys[74] = true;
            break;
        case 0xCC:
            *keychar = '5';
            *keystr = (char*) "Keypad 5";
            *isKeyPressed = false;
            keys[74] = false;
            break;
        case 0x4D:
            *keychar = '6';
            *keystr = (char*) "Keypad 6";
            *isKeyPressed = true;
            keys[75] = true;
            break;
        case 0xCD:
            *keychar = '6';
            *keystr = (char*) "Keypad 6";
            *isKeyPressed = false;
            keys[75] = false;
            break;
        case 0x4E:
            *keychar = '+';
            *keystr = (char*) "Keypad +";
            *isKeyPressed = true;
            keys[76] = true;
            break;
        case 0xCE:
            *keychar = '+';
            *keystr = (char*) "Keypad +";
            *isKeyPressed = false;
            keys[76] = false;
            break;
        case 0x4F:
            *keychar = '1';
            *keystr = (char*) "Keypad 1";
            *isKeyPressed = true;
            keys[77] = true;
            break;
        case 0xCF:
            *keychar = '1';
            *keystr = (char*) "Keypad 1";
            *isKeyPressed = false;
            keys[77] = false;
            break;
        case 0x50:
            *keychar = '2';
            *keystr = (char*) "Keypad 2";
            *isKeyPressed = true;
            keys[78] = true;
            break;
        case 0xD0:
            *keychar = '2';
            *keystr = (char*) "Keypad 2";
            *isKeyPressed = false;
            keys[78] = false;
            break;
        case 0x51:
            *keychar = '3';
            *keystr = (char*) "Keypad 3";
            *isKeyPressed = true;
            keys[79] = true;
            break;
        case 0xD1:
            *keychar = '3';
            *keystr = (char*) "Keypad 3";
            *isKeyPressed = false;
            keys[79] = false;
            break;
        case 0x52:
            *keychar = '0';
            *keystr = (char*) "Keypad 0";
            *isKeyPressed = true;
            keys[80] = true;
            break;
        case 0xD2:
            *keychar = '0';
            *keystr = (char*) "Keypad 0";
            *isKeyPressed = false;
            keys[80] = false;
            break;
        case 0x53:
            *keychar = '.';
            *keystr = (char*) "Keypad .";
            *isKeyPressed = true;
            keys[81] = true;
            break;
        case 0xD3:
            *keychar = '.';
            *keystr = (char*) "Keypad .";
            *isKeyPressed = false;
            keys[81] = false;
            break;
        case 0x55:
            *keychar = '\0';
            *keystr = (char*) "F11";
            *isKeyPressed = true;
            keys[82] = true;
            break;
        case 0xD5:
            *keychar = '\0';
            *keystr = (char*) "F11";
            *isKeyPressed = false;
            keys[82] = false;
            break;
        case 0x56:
            *keychar = '\0';
            *keystr = (char*) "Unlabeled";
            *isKeyPressed = true;
            keys[83] = true;
            break;
        case 0xD6:
            *keychar = '\0';
            *keystr = (char*) "Unlabeled";
            *isKeyPressed = false;
            keys[83] = false;
            break;
        case 0x57:
            *keychar = '\0';
            *keystr = (char*) "F11";
            *isKeyPressed = true;
            keys[84] = true;
            break;
        case 0xD7:
            *keychar = '\0';
            *keystr = (char*) "F11";
            *isKeyPressed = false;
            keys[84] = false;
            break;
        case 0x58:
            *keychar = '\0';
            *keystr = (char*) "F12";
            *isKeyPressed = true;
            keys[85] = true;
            break;
        case 0xD8:
            *keychar = '\0';
            *keystr = (char*) "F12";
            *isKeyPressed = false;
            keys[85] = false;
            break;
        default:
            *keychar = '\0';
            *keystr = (char*) "";
            *isKeyPressed = false;
            break;
    }
    return *keystr;
}

char updateKey(uint8_t keycode) {
    char keychar, *keystr;
    bool isKeyPressed;
    getInfo(keycode, &keychar, &keystr, &isKeyPressed);
    if (keys[27] && (keys[52] || keys[40]) && keys[0]) {
        // Pull up display
        gui->printf("TEST");
        return ' ';
    }
    if (keys[52] || keys[40]) shifted = true;
    else if (!keys[52] && !keys[40]) shifted = false;

    char c = '\0';
    switch (keychar) {
        case '`':
            if (shifted) c = '~';
            else c = '`';
            break;
        case '1':
            if (shifted) c = '!';
            else c = '1';
            break;
        case '2':
            if (shifted) c = '@';
            else c = '2';
            break;
        case '3':
            if (shifted) c = '#';
            else c = '3';
            break;
        case '4':
            if (shifted) c = '$';
            else c = '4';
            break;
        case '5':
            if (shifted) c = '%';
            else c = '5';
            break;
        case '6':
            if (shifted) c = '^';
            else c = '6';
            break;
        case '7':
            if (shifted) c = '&';
            else c = '7';
            break;
        case '8':
            if (shifted) c = '*';
            else c = '8';
            break;
        case '9':
            if (shifted) c = '(';
            else c = '9';
            break;
        case '0':
            if (shifted) c = ')';
            else c = '0';
            break;
        case '-':
            if (shifted) c = '_';
            else c = '-';
            break;
        case '=':
            if (shifted) c = '+';
            else c = '=';
            break;
        case '\t':
            if (shifted) c = '\t';
            else c = '\t';
            break;
        case 'Q':
            if (shifted) c = 'Q';
            else c = 'q';
            break;
        case 'W':
            if (shifted) c = 'W';
            else c = 'w';
            break;
        case 'E':
            if (shifted) c = 'E';
            else c = 'e';
            break;
        case 'R':
            if (shifted) c = 'R';
            else c = 'r';
            break;
        case 'T':
            if (shifted) c = 'T';
            else c = 't';
            break;
        case 'Y':
            if (shifted) c = 'Y';
            else c = 'y';
            break;
        case 'U':
            if (shifted) c = 'U';
            else c = 'u';
            break;
        case 'I':
            if (shifted) c = 'I';
            else c = 'i';
            break;
        case 'O':
            if (shifted) c = 'O';
            else c = 'o';
            break;
        case 'P':
            if (shifted) c = 'P';
            else c = 'p';
            break;
        case '[':
            if (shifted) c = '{';
            else c = '[';
            break;
        case ']':
            if (shifted) c = '}';
            else c = ']';
            break;
        case '\\':
            if (shifted) c = '|';
            else c = '\\';
            break;
        case 'A':
            if (shifted) c = 'A';
            else c = 'a';
            break;
        case 'S':
            if (shifted) c = 'S';
            else c = 's';
            break;
        case 'D':
            if (shifted) c = 'D';
            else c = 'd';
            break;
        case 'F':
            if (shifted) c = 'F';
            else c = 'f';
            break;
        case 'G':
            if (shifted) c = 'G';
            else c = 'g';
            break;
        case 'H':
            if (shifted) c = 'H';
            else c = 'h';
            break;
        case 'J':
            if (shifted) c = 'J';
            else c = 'j';
            break;
        case 'K':
            if (shifted) c = 'K';
            else c = 'k';
            break;
        case 'L':
            if (shifted) c = 'L';
            else c = 'l';
            break;
        case ';':
            if (shifted) c = ':';
            else c = ';';
            break;
        case '\'':
            if (shifted) c = '"';
            else c = '\'';
            break;
        case 'Z':
            if (shifted) c = 'Z';
            else c = 'z';
            break;
        case 'X':
            if (shifted) c = 'X';
            else c = 'x';
            break;
        case 'C':
            if (shifted) c = 'C';
            else c = 'c';
            break;
        case 'V':
            if (shifted) c = 'V';
            else c = 'v';
            break;
        case 'B':
            if (shifted) c = 'B';
            else c = 'b';
            break;
        case 'N':
            if (shifted) c = 'N';
            else c = 'n';
            break;
        case 'M':
            if (shifted) c = 'M';
            else c = 'm';
            break;
        case ',':
            if (shifted) c = '<';
            else c = ',';
            break;
        case '.':
            if (shifted) c = '>';
            else c = '.';
            break;
        case '/':
            if (shifted) c = '?';
            else c = '/';
            break;
        case ' ':
            c = ' ';
            break;
        case '\n':
            c = '\n';
            break;
        default:
            c = '\0';
            break;
    }
    
    return c;
}