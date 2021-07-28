#include "input/keyboard.h"

bool isLeftShift;
bool isRightShift;

uint64_t index = 0;
bool CanWrite;
bool CanErease;
char kbBuffer[100];
uint8_t kbScancode;

void HandleKeyboard() {
    if(CanWrite) {
        switch(kbScancode) {
            case LeftShift:
                isLeftShift = true;
                return;
            case LeftShift + 0x80: // rightshift
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
                if(CanErease){
                    kbBuffer[index - 1] = '\0';
                    renderer->clearChar();
                }
                return;
            case Spacebar:
                renderer->putChar(' ');
                return;
        }
    }
    
    char ascii = QWERTYKeyboard::Translate(kbScancode, isLeftShift | isRightShift);
    if (ascii != 0) {
        if(CanWrite){
            kbBuffer[index] = ascii;
            renderer->putChar(ascii);                
            index++;
        }
    }
}

void clearBuffer(char buffer[]) {
    for(int i = 0; buffer[i] != '\0'; i++)
        buffer[i] = ' ';
    index = 0;
}