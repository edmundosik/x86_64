#include "panic.h"

void panic(const char* panicMessage) {
    renderer->clearColor = 0xff000082;
    renderer->clear();
    renderer->CursorPosition = {0, 0};
    renderer->color = 0xffffffff;

    renderer->nextLine();
    renderer->print("A problem has detected and Windows has been shut down to prevent damage");
    renderer->nextLine();
    renderer->print("to your computer.");
    renderer->nextLine();
    renderer->nextLine();
    renderer->print(panicMessage);
    renderer->nextLine();
    renderer->nextLine();
    renderer->print("If this is the first time you've seen this Stop error screen,");
    renderer->nextLine();
    renderer->print("restart your computer. If this screen appears again, follow");
    renderer->nextLine();
    renderer->print("these steps:");
    renderer->nextLine();
    renderer->nextLine();
    renderer->print("Check to make sure any new hardware or software is properly installed.");
    renderer->nextLine();
    renderer->print("If this is a new ubstakkatuib, ask your hardware or software manufacturer");
    renderer->nextLine();
    renderer->print("for any Windows updates you might need.");
    renderer->nextLine();
    renderer->nextLine();
    renderer->print("If problems continue, disable or remove any newly installed hardware");
    renderer->nextLine();
    renderer->print("or software. Disable BIOS memory options such as caching or shadowing.");
    renderer->nextLine();
    renderer->print("If you need to use Safe Mode to remove or disable components, restart");
    renderer->nextLine();
    renderer->print("your computer, press F8 to select Advanced Startup Options, and then");
    renderer->nextLine();
    renderer->print("select Safe Mode.");
    renderer->nextLine();
    renderer->nextLine();
    renderer->print("Technical information:");
    renderer->nextLine();
    renderer->nextLine();
    renderer->print("*** STOP: 0x00000000 (0x00000000, 0x00000000, 0x00000000, 0x00000000)"); // TODO: display additional info
}