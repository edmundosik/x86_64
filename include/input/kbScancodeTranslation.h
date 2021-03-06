#pragma once
#include <stdint.h>

namespace QWERTYKeyboard {
    #define LeftShift 0x2a
    #define RightShift 0x36
    #define Enter 0x1c
    #define Backspace 0x0e
    #define Spacebar 0x39

    extern const char ASCIITable[];
    char Translate(uint8_t scancode, bool uppercase);
};