#include "gfx/BasicRenderer.h"

BasicRenderer* renderer;

BasicRenderer::BasicRenderer(FrameBuffer* targetFrameBuffer, PSF1_FONT* PSF1_font) {
    TargetFrameBuffer = targetFrameBuffer;
    PSF1_Font = PSF1_font;
    color = 0xffffffff;
    CursorPosition = {0, 0};
}

void BasicRenderer::print(const char* str) {
	char* chr = (char*)str;
    while(*chr != 0) {
        if(CursorPosition.Y + 16 > TargetFrameBuffer->Height) { // TODO: Handle scrolling to get rid of Page fault
			CursorPosition.Y = 0;
			clear();
		}
        putChar(*chr, CursorPosition.X, CursorPosition.Y);
        CursorPosition.X += 8;
        if(CursorPosition.X + 8 > TargetFrameBuffer->Width) {
            CursorPosition.X = 0;
            CursorPosition.Y += 16;
        }
        chr++;
    }
}

void BasicRenderer::print(const char* str, Point pos) {
	char* chr = (char*)str;
    while(*chr != 0) {
        if(pos.Y + 16 > TargetFrameBuffer->Height) { // TODO: Handle scrolling to get rid of Page fault
			pos.Y = 0;
			clear();
		}
        putChar(*chr, pos.X, pos.Y);
        pos.X += 8;
        if(pos.X + 8 > TargetFrameBuffer->Width) {
            pos.X = 0;
            pos.Y += 16;
        }
        chr++;
    }
}

void BasicRenderer::putChar(char chr, unsigned int xOffset, unsigned int yOffset) {
	unsigned int* pixPtr = (unsigned int*)TargetFrameBuffer->BaseAddress;
	char* fontPtr = (char*)PSF1_Font->glyphBuffer + (chr * PSF1_Font->psf1_Header->charsize);

    if(renderer->CursorPosition.Y + 16 > renderer->TargetFrameBuffer->Height) { // TODO: Handle scrolling to get rid of Page fault
			renderer->CursorPosition.Y = 0;
			renderer->clear();
	}

	for (unsigned long y = yOffset; y < yOffset + 16; y++) {
		for (unsigned long x = xOffset; x < xOffset + 8; x++) {
			if((*fontPtr & (0b10000000 >> (x - xOffset))) > 0) {
				*(unsigned int*)(pixPtr + x + (y * TargetFrameBuffer->PixelsPerScanline)) = color;
			}
		}
		fontPtr++;
	}
	
}

void BasicRenderer::putChar(char chr) {
	putChar(chr, CursorPosition.X, CursorPosition.Y);
    CursorPosition.X += 8;
    if (CursorPosition.X + 8 > TargetFrameBuffer->Width)
        CursorPosition = {0, CursorPosition.Y += 16};
	
}

void BasicRenderer::clear() {
    uint64_t fbBase = (uint64_t)TargetFrameBuffer->BaseAddress;
    uint64_t fbSize = TargetFrameBuffer->BufferSize;
    uint64_t bytesPerScanline = TargetFrameBuffer->PixelsPerScanline * 4;
    uint64_t fbHeight = TargetFrameBuffer->Height;
    
    for(int vertScanLine = 0; vertScanLine < fbHeight; vertScanLine++) {
        uint64_t pixPtrBase = fbBase + (bytesPerScanline * vertScanLine);
        for (uint32_t* pixPtr = (uint32_t*)pixPtrBase; pixPtr < (uint32_t*)(pixPtrBase + bytesPerScanline); pixPtr++) {
            *pixPtr = clearColor;
        }
    }
}

void BasicRenderer::nextLine() {
    CursorPosition = {0, CursorPosition.Y += 16};
}

void BasicRenderer::clearChar() {
    if(CursorPosition.X == 0) {
        CursorPosition.X = TargetFrameBuffer->Width;
        CursorPosition.Y -= 16;
        if(CursorPosition.Y < 0)
            CursorPosition.Y = 0;
    }

    unsigned int* pixPtr = (unsigned int*)TargetFrameBuffer->BaseAddress;
    for (unsigned long y = CursorPosition.Y; y < CursorPosition.Y + 16; y++) 
		for (unsigned long x = CursorPosition.X - 8; x < CursorPosition.X; x++) 
				*(unsigned int*)(pixPtr + x + (y * TargetFrameBuffer->PixelsPerScanline)) = clearColor;

    CursorPosition.X -= 8;
    if(CursorPosition.X < 0) {
        CursorPosition.X = TargetFrameBuffer->Width;
        CursorPosition.Y -= 16;
        if(CursorPosition.Y < 0) 
            CursorPosition.Y = 0;
    }
}

void BasicRenderer::putPix(Point pos, uint32_t color) {
    *(uint32_t*)((uint64_t)TargetFrameBuffer->BaseAddress + (pos.X * 4) + (pos.Y * TargetFrameBuffer->PixelsPerScanline * 4)) = color;
}

uint32_t BasicRenderer::getPix(uint32_t x, uint32_t y) {
    return *(uint32_t*)((uint64_t)TargetFrameBuffer->BaseAddress + (x * 4) + (y * TargetFrameBuffer->PixelsPerScanline * 4));
}

void BasicRenderer::clearMouseCursor(uint8_t* mouseCursor, Point position) {
    if(!mouseDrawn)
        return;

    int xMax = 16;
    int yMax = 16;
    int differenceX = TargetFrameBuffer->Width - position.X;
    int differenceY = TargetFrameBuffer->Height - position.Y;

    if(differenceX < xMax)
        xMax = differenceX;
    if(differenceY < yMax)
        yMax = differenceY;
    
    for(int y = 0; y < yMax; y++) {
        for(int x = 0; x < xMax; x++) {
            int bit = y * 16 + x;
            int byte = bit / 8;
            if((mouseCursor[byte] & (0b10000000 >> (x % 8)))) {
                if(getPix(position.X + x, position.Y + y) == MouseCursorBufferAfter[x + y * 16])
                    putPix({position.X + x, position.Y + y}, MouseCursorBuffer[x + y * 16]);
            }
        }
    }
}

void BasicRenderer::drawOverlayMouseCursor(uint8_t* mouseCursor, Point position, uint32_t color) {
    int xMax = 16;
    int yMax = 16;
    int differenceX = TargetFrameBuffer->Width - position.X;
    int differenceY = TargetFrameBuffer->Height - position.Y;

    if(differenceX < xMax)
        xMax = differenceX;
    if(differenceY < yMax)
        yMax = differenceY;
    
    for(int y = 0; y < yMax; y++) {
        for(int x = 0; x < xMax; x++) {
            int bit = y * 16 + x;
            int byte = bit / 8;
            if((mouseCursor[byte] & (0b10000000 >> (x % 8)))) {
                MouseCursorBuffer[x + y * 16] = getPix(position.X + x, position.Y + y);
                putPix({position.X + x, position.Y + y}, color);
                MouseCursorBufferAfter[x + y * 16] = getPix(position.X + x, position.Y + y);
            }
        }
    }
    mouseDrawn = true;
}

void BasicRenderer::drawCursor() {
    putChar('_', CursorPosition.X, CursorPosition.Y);
}

void BasicRenderer::cursorBlink() {
    if(cursor)
        drawCursor();
}

void BasicRenderer::printWithShadow(const char* str, uint32_t shadowColor, long shadowDistance) {
    Point oldCursorPos = CursorPosition;
    uint32_t oldColor = color;

    color = shadowColor;
    CursorPosition = {CursorPosition.X + shadowDistance, CursorPosition.Y + shadowDistance};
    print(str);

    color = oldColor;
    CursorPosition = oldCursorPos;
    print(str);
}

void BasicRenderer::rect(Point position, Point size, uint32_t color) {
    int i, j;

    for(i = 0; i < size.X; i++) {
        for(j = 0; j < size.Y; j++) {
            putPix({position.X + i, position.Y + j}, color);
        }
    }
}

void BasicRenderer::draw_icon(Point pos, Point size, uint64_t pixels[]) {
    int i, j, l;
    for (l = j = 0; l < size.X; l++) {
        for (i = 0; i < size.Y; i++, j++) {
            putPix({pos.X + i, pos.Y + l}, pixels[j]);
        }
    }
}