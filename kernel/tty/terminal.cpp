#include "tty/terminal.h"

void crash() {
    clearBuffer(kbBuffer);
    int* pagefault = (int*)0x80000000000;
	*pagefault = 2;
}

void clear() {
    clearBuffer(kbBuffer);
    renderer->clear();
    renderer->CursorPosition = {0, 0};
}

void memory() {
    clearBuffer(kbBuffer);
    renderer->print("Free RAM: ");
    renderer->print(to_string(PageAllocator.GetFreeRAM() / 1048576));
    renderer->print(" MB");
    renderer->nextLine();
    renderer->print("Used RAM: ");
    renderer->print(to_string(PageAllocator.GetUsedRAM() / 1048576));
    renderer->print(" MB");
    renderer->nextLine();
    renderer->print("Reserved RAM: ");
    renderer->print(to_string(PageAllocator.GetReservedRAM() / 1048576));
    renderer->print(" MB");
}

void help() {
    clearBuffer(kbBuffer);
    renderer->print("cls or clear   - clear screen");
    renderer->nextLine();
    renderer->print("crash          - make system crash (PAGE_FAULT)");
    renderer->nextLine();
    renderer->print("memory         - display memory info");
    renderer->nextLine();
    renderer->print("shadowtest     - write text with shadow");
    renderer->nextLine();
    renderer->print("display        - display screen info");
    renderer->nextLine();
    renderer->print("rect           - draws green rect somewhere at screen");
    renderer->nextLine();
    renderer->print("devices        - display PCI devices info");
}

void error() {
    clearBuffer(kbBuffer);
    uint32_t oldColor = renderer->color;
    renderer->color = 0xffff0000;
    renderer->print("ERROR: ");
    renderer->color = oldColor;
    renderer->print("Unknown command!");
}

void shadowtest() {
    clearBuffer(kbBuffer);
    renderer->printWithShadow("Text with Shadow! with 1px distance", 0xff797a7a, 1);
    renderer->nextLine();
    renderer->printWithShadow("Text with Shadow! with 2px distance", 0xff797a7a, 2);
    renderer->nextLine();
    renderer->printWithShadow("Text with Shadow! with 3px distance", 0xff797a7a, 3);
    renderer->nextLine();
    renderer->printWithShadow("Text with Shadow! with 4px distance", 0xff797a7a, 4);
    renderer->nextLine();
    renderer->printWithShadow("Text with Shadow! with 5px distance", 0xff797a7a, 5);
    renderer->nextLine();
    renderer->printWithShadow("Text with Shadow! with 6px distance", 0xff797a7a, 6);
    renderer->nextLine();
    renderer->printWithShadow("Text with Shadow! with 7px distance", 0xff797a7a, 7);
    renderer->nextLine();
    renderer->printWithShadow("Text with Shadow! with 8px distance", 0xff797a7a, 8);
    renderer->nextLine();
    renderer->printWithShadow("Text with Shadow! with 9px distance", 0xff797a7a, 9);
    renderer->nextLine();
    renderer->printWithShadow("Text with Shadow! with 10px distance", 0xff797a7a, 10);
}

void display() {
    clearBuffer(kbBuffer);
    renderer->print("Resolution: ");
    renderer->print(to_string((uint64_t)renderer->TargetFrameBuffer->Width));
    renderer->print(" x ");
    renderer->print(to_string((uint64_t)renderer->TargetFrameBuffer->Height));
    renderer->nextLine();
    renderer->print("Pixels per scanline: ");
    renderer->print(to_string((uint64_t)renderer->TargetFrameBuffer->PixelsPerScanline));
    renderer->nextLine();
    renderer->print("Framebuffer address: 0x");
    renderer->print(hex_to_string((uint64_t)renderer->TargetFrameBuffer->BaseAddress));
    renderer->nextLine();
    renderer->print("Framebuffer size: 0x");
    renderer->print(hex_to_string((uint32_t)renderer->TargetFrameBuffer->BufferSize));
}

void rect() {
    clearBuffer(kbBuffer);
    renderer->rect({50, 100}, {15, 15}, 0xff00ff00);
}

void icontest() {
    clearBuffer(kbBuffer);
    uint8_t icon[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    uint8_t icon2[] = {
        0b10000000, 0b00000000,
        0b11000000, 0b00000000,
        0b10100000, 0b00000000,
        0b10010000, 0b00000000,
        0b10001000, 0b00000000,
        0b10000100, 0b00000000,
        0b10000010, 0b00000000,
        0b10000001, 0b00000000,
        0b10000000, 0b10000000,
        0b10000011, 0b00000000,
        0b10000100, 0b00000000,
        0b10110010, 0b00000000,
        0b11001010, 0b00000000,
        0b10001001, 0b00000000,
        0b00000101, 0b00000000,
        0b00000010, 0b00000000
    };

    uint8_t array[]={0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

    renderer->draw_icon({150, 200}, {16, 16}, array);
}

void devices() {
    clearBuffer(kbBuffer);
    renderer->print("VendorID / DeviceID / Class / Subclass / ProgIF");
    renderer->nextLine();
    PCI::EnumeratePCI(mcfg);
}

void Terminal::InitTTY() {
    renderer->cursor = true;
    clearBuffer(kbBuffer);
    CanWrite = true;
    CanErease = true;

    doCommand = false;
    isReady = true;

    renderer->print("Terminal successfully initialized.");
	renderer->nextLine();
    renderer->color = prefixColor;
    renderer->print(prefix);
    renderer->color = textColor;
}

void Terminal::loop() {
    // if(kbBuffer[0] == '\0' || kbBuffer[1] == '\0')
    //     CanErease = false;
    // else
    //     CanErease = true;

    switch (kbScancode) {
        case Enter:
            doCommand = true;
            return;
    }

    if(doCommand) {
        isReady = false;
        if(strcmp(charBufferToString(), (char*)"crash", 5)) {
            crash();
        } else if(strcmp(charBufferToString(), (char*)"clear", 5)) {
            clear();
        } else if(strcmp(charBufferToString(), (char*)"memory", 6)) {
            memory();
        } else if(strcmp(charBufferToString(), (char*)"help", 4)) {
            help();
        } else if(strcmp(charBufferToString(), (char*)"shadowtest", 10)) {
            shadowtest();
        } else if(strcmp(charBufferToString(), (char*)"display", 7)) {
            display();
        } else if(strcmp(charBufferToString(), (char*)"rect", 4)) {
            rect();
        } else if(strcmp(charBufferToString(), (char*)"icontest", 8)) {
            icontest();
        } else if(strcmp(charBufferToString(), (char*)"devices", 7)) {
            devices();
        } else {
            error();
        }

        renderer->nextLine();
        renderer->color = prefixColor;
        renderer->print(prefix);
        renderer->color = textColor;
        clearBuffer(kbBuffer);
        doCommand = false;
        isReady = true;
    }

    if(!isReady) {
        CanWrite = false;
    }
    
}

char* Terminal::charBufferToString(){
    char* out = (char*)"";
    for(int i = 0; kbBuffer[i] != '\0'; i++) {
        out[i] = kbBuffer[i];
    }
    return out;
}