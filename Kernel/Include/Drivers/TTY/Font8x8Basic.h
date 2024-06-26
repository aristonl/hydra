#ifndef FONT8X8_BASIC_H
#define FONT8X8_BASIC_H

#include <stdint.h>

const uint8_t font8x8_basic[128][8] = {
    // ASCII 0-31 (Control characters, not defined here)
    {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0},
    // ASCII 32 (Space)
    {0,0,0,0,0,0,0,0},
    // ASCII 33 (Exclamation mark)
    {24,24,24,24,24,0,24,0},
    // ASCII 34 (Quotation mark)
    {54,54,20,0,0,0,0,0},
    // ASCII 35 (Hash)
    {54,54,127,54,127,54,54,0},
    // ASCII 36 (Dollar)
    {8,62,40,62,10,62,8,0},
    // ASCII 37 (Percent)
    {0,99,51,24,12,102,99,0},
    // ASCII 38 (Ampersand)
    {28,54,28,119,51,51,110,0},
    // ASCII 39 (Single quote)
    {6,6,3,0,0,0,0,0},
    // ASCII 40 (Open parenthesis)
    {12,6,3,3,3,6,12,0},
    // ASCII 41 (Close parenthesis)
    {3,6,12,12,12,6,3,0},
    // ASCII 42 (Asterisk)
    {0,102,60,255,60,102,0,0},
    // ASCII 43 (Plus)
    {0,24,24,126,24,24,0,0},
    // ASCII 44 (Comma)
    {0,0,0,0,0,12,12,6},
    // ASCII 45 (Minus)
    {0,0,0,126,0,0,0,0},
    // ASCII 46 (Period)
    {0,0,0,0,0,12,12,0},
    // ASCII 47 (Slash)
    {96,48,24,12,6,3,1,0},
    // ASCII 48-57 (Numbers 0-9)
    {62,99,103,111,123,115,99,0}, {12,14,12,12,12,12,63,0}, {62,99,96,48,24,12,127,0}, {62,99,48,28,48,99,62,0}, {56,60,54,51,127,48,120,0}, {127,3,63,96,96,99,62,0}, {62,99,3,63,99,99,62,0}, {127,96,48,24,12,12,12,0}, {62,99,99,62,99,99,62,0}, {62,99,99,126,96,99,62,0},
    // ASCII 58 (Colon)
    {0,12,12,0,0,12,12,0},
    // ASCII 59 (Semicolon)
    {0,12,12,0,0,12,12,6},
    // ASCII 60 (Less than)
    {24,12,6,3,6,12,24,0},
    // ASCII 61 (Equals)
    {0,0,126,0,0,126,0,0},
    // ASCII 62 (Greater than)
    {6,12,24,48,24,12,6,0},
    // ASCII 63 (Question mark)
    {62,99,48,24,12,0,12,0},
    // ASCII 64 (At)
    {62,99,99,111,111,3,62,0},
    // ASCII 65-90 (Uppercase A-Z)
    {0,24,36,66,126,66,66,0}, {63,66,66,62,66,66,63,0}, {60,66,3,3,3,66,60,0}, {31,34,66,66,66,34,31,0}, {127,1,1,63,1,1,127,0}, {127,1,1,63,1,1,1,0}, {60,66,3,3,99,66,124,0},
    {66,66,66,126,66,66,66,0}, {28,8,8,8,8,8,28,0}, {48,32,32,32,32,34,60,0}, {67,18,10,6,10,18,67,0}, {1,1,1,1,1,1,127,0}, {65,99,85,73,65,65,65,0}, {65,97,81,73,69,67,65,0},
    {28,34,65,65,65,34,28,0}, {63,66,66,62,2,2,2,0}, {28,34,65,65,81,34,92,0}, {63,66,66,62,10,18,34,0}, {62,65,1,62,64,65,62,0}, {127,8,8,8,8,8,8,0}, {66,66,66,66,66,66,126,0},
    {66,66,66,66,66,36,24,0}, {65,65,65,73,85,99,65,0}, {66,66,36,24,36,66,66,0}, {65,34,20,8,8,8,8,0}, {127,64,32,16,8,4,127,0},
    // ASCII 91-96 (Brackets and punctuation)
    {14,2,2,2,2,2,14,0}, {0,1,2,4,8,16,32,0}, {14,8,8,8,8,8,14,0}, {8,20,34,65,0,0,0,0}, {0,0,0,0,0,0,0,255}, {3,3,6,12,24,48,96,0},
    // ASCII 97-122 (Lowercase a-z)
    {0,0,60,96,124,66,124,0}, {3,3,3,31,35,35,31,0}, {0,0,62,3,3,66,60,0}, {48,48,48,124,66,66,124,0}, {0,0,60,66,127,3,60,0}, {28,2,2,31,2,2,2,0}, {0,0,124,66,124,48,66,60},
    {3,3,3,31,35,35,35,0}, {8,0,14,8,8,8,28,0}, {16,0,28,16,16,18,14,0}, {3,3,35,19,11,19,35,0}, {14,8,8,8,8,8,28,0}, {0,0,26,37,37,65,65,0}, {0,0,31,33,33,33,33,0},
    {0,0,30,33,33,33,30,0}, {0,0,31,33,31,3,3,0}, {0,0,60,66,124,48,48,0}, {0,0,62,3,3,3,3,0}, {0,0,62,64,60,2,124,0}, {4,4,31,4,4,4,24,0}, {0,0,66,66,66,66,124,0},
    {0,0,66,66,66,36,24,0}, {0,0,65,65,85,85,34,0}, {0,0,65,34,28,34,65,0}, {0,0,66,36,24,24,24,0}, {0,0,127,48,24,12,127,0},
    // ASCII 123-127 (Braces, pipe, tilde, DEL)
    {12,6,6,3,6,6,12,0}, {8,8,8,8,8,8,8,0}, {3,6,6,12,6,6,3,0}, {50,84,8,0,0,0,0,0}, {0,0,0,0,0,0,0,0}
};

#endif // FONT8X8_BASIC_H

