#include "console/console.h"

char* buffer;

void crash() {
    buffer = 0;
    int* pagefault = (int*)0x80000000000;
	*pagefault = 2;
}

void clear() {
    buffer = 0;
    renderer->clear();
    renderer->CursorPosition = {0, 0};
}

void memory() {
    buffer = 0;
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
    buffer = 0;
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
    buffer = 0;
    uint32_t oldColor = renderer->color;
    renderer->color = 0xffff0000;
    renderer->print("ERROR: ");
    renderer->color = oldColor;
    renderer->print("Unknown command!");
    renderer->nextLine();
}

void shadowtest() {
    buffer = 0;
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
    buffer = 0;
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
    buffer = 0;
    renderer->rect({50, 100}, {15, 15}, 0xff00ff00);
}

void devices() {
    buffer = 0;
    renderer->print("VendorID / DeviceID / Class / Subclass / ProgIF");
    renderer->nextLine();
    PCI::EnumeratePCI(mcfg);
}

char* getInput() {
    char* output = 0;
    char tmp = 0;
    int len = 0;;

    while((tmp = GetKey()) != '\n') {
        if(tmp != '\b') {
            output[len] = tmp;
            len++;
        } else if(len > 0) {
            len--;
            output[len] = 0;
        }
    }

    output[len] = 0;
    return output;
}

void Console::InitCMD() {
    malloc(0x80000);
    renderer->cursor = true;
    //renderer->print(charBufferToString());
    renderer->nextLine();
    //renderer->print(charBufferToString());
    // memset(&kbBuffer[0], 0, sizeof(kbBuffer));
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

void Console::loop() {
    buffer = getInput();

    switch (kbScancode) {
        case Enter:
            doCommand = true;
            return;
    }

    if(doCommand) {
        isReady = false;
        if(strcmp(getInput(), (char*)"crash")) {
            crash();
        } else if(strcmp(getInput(), (char*)"clear")) {
            clear();
        } else if(strcmp(getInput(), (char*)"memory")) {
            memory();
        } else if(strcmp(getInput(), (char*)"help")) {
            help();
        } else if(strcmp(getInput(), (char*)"shadowtest")) {
            shadowtest();
        } else if(strcmp(getInput(), (char*)"display")) {
            display();
        } else if(strcmp(getInput(), (char*)"rect")) {
            rect();
        } else if(strcmp(getInput(), (char*)"devices")) {
            devices();
        } else {
            error();
            // memset(&kbBuffer[0], 0, kbBuffer[100]);
            // renderer->print(charBufferToString());
            renderer->print(getInput());
        }

        renderer->nextLine();
        renderer->color = prefixColor;
        renderer->print(prefix);
        renderer->color = textColor;
        buffer = 0;
        doCommand = false;
        isReady = true;
    }

    if(!isReady) {
        CanWrite = false;
    }
    
}