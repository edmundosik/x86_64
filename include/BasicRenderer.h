#pragma once
#include <stdint.h>
#include "math.h"
#include "FrameBuffer.h"
#include "SimpleFonts.h"

class BasicRenderer{
    public:
        BasicRenderer(FrameBuffer* TargetFrameBuffer, PSF1_FONT* PSF1_Font);
        Point CursorPosition;
        FrameBuffer* TargetFrameBuffer;
        PSF1_FONT* PSF1_Font;
        unsigned int color;
        void print(const char* str);
        void putChar(char chr, unsigned int xOffset, unsigned int yOffset);
        void clear(uint32_t color);
        void nextLine();
};

extern BasicRenderer* renderer;