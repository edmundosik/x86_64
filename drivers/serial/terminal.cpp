#include "serial/terminal.h"

void Terminal::print(const char* text) {
    while(*text != 0) {
        SerialWrite(*text++);
    }
}