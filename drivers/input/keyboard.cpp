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
                    // kbBuffer[index - 1] = 0;
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
            renderer->putChar(ascii);

            // kbBuffer[index] = ascii;          
            // index++;
        }
    }
}

char GetKey() {
    if(kbScancode != 0) {
        return QWERTYKeyboard::Translate(kbScancode, isLeftShift | isRightShift);
    }
    return 0;
}

void clearBuffer(char buffer[]) {
    for(int i = 0; buffer[i] != 0; i++)
        buffer[i] = 0;
    //index = 0;

    //memset(&buffer[0], 0, sizeof(buffer));

    // int i = 0;
    // while(buffer[i] != '\0') {
    //     buffer[i] = '\0';
    //     i++;
    // }
}