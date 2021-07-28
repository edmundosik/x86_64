#pragma once
#include <stdint.h>
#include "kernelUtil.h"

class Terminal {
    public:
        char* prefix;
        uint32_t prefixColor;
        uint32_t textColor;
        void InitTTY();
        void loop();
    private:
        bool isReady;
        bool doCommand;
        char* charBufferToString();
};

void crash();
void clear();
void memory();
void help();
void error();
void shadowtest();
void devices();