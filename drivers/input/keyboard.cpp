#include "input/keyboard.h"

bool isLeftShift;
bool isRightShift;

void HandleKeyboard(uint8_t scancode) {
    switch(scancode) {
        case LeftShift:
            isLeftShift = true;
            return;
        case LeftShift + 0x80:
            isLeftShift = false;
            return;
        case RightShift:
            isRightShift = true;
            return;
        case RightShift + 0x80:
            isRightShift = false;
            return;
        case Enter:
            renderer->nextLine();
            return;
        case Backspace:
            renderer->clearChar();
            return;
        case Spacebar:
            renderer->putChar(' ');
            return;
    }

    char ascii = QWERTYKeyboard::Translate(scancode, isLeftShift | isRightShift);
    if (ascii != 0) {
        renderer->putChar(ascii);
    }
}