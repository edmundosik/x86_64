#pragma once
#include <stdint.h>
#include "math.h"
#include "gfx/FrameBuffer.h"
#include "gfx/SimpleFonts.h"

class BasicRenderer{
    public:
        BasicRenderer(FrameBuffer* TargetFrameBuffer, PSF1_FONT* PSF1_Font);
        Point CursorPosition;
        FrameBuffer* TargetFrameBuffer;
        PSF1_FONT* PSF1_Font;
        uint32_t MouseCursorBuffer[16 * 16];
        uint32_t MouseCursorBufferAfter[16 * 16];
        unsigned int color;
        unsigned int clearColor;
        void print(const char* str);
        void putChar(char chr, unsigned int xOffset, unsigned int yOffset);
        void putChar(char chr);
        void clear();
        void nextLine();
        void clearChar();
        void putPix(Point pos, uint32_t color);
        uint32_t getPix(uint32_t x, uint32_t y);
        void clearMouseCursor(uint8_t* mouseCursor, Point position);
        void drawOverlayMouseCursor(uint8_t* mouseCursor, Point position, uint32_t color);
        bool mouseDrawn;
        void drawCursor();
        bool cursor;
        void cursorBlink();
        void printWithShadow(const char* str, uint32_t shadowColor, long shadowDistance);
        void rect(Point position, Point size, uint32_t color);
        void draw_icon(Point pos, Point size, uint8_t pixels[]);
};

extern BasicRenderer* renderer;