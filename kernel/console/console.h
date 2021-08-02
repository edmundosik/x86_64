#pragma once
#include <stdint.h>
#include "string.h"
#include "kernelUtil.h"

class Console {
    public:
        char* prefix;
        uint32_t prefixColor;
        uint32_t textColor;
        void InitCMD();
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