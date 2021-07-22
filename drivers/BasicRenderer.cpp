#include <BasicRenderer.h>

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
        putChar(*chr, CursorPosition.X, CursorPosition.Y);
        CursorPosition.X += 8;
        if(CursorPosition.X + 8 > TargetFrameBuffer->Width) {
            CursorPosition.X = 0;
            CursorPosition.Y += 16;
        }
        chr++;
    }
}

void BasicRenderer::putChar(char chr, unsigned int xOffset, unsigned int yOffset) {
	unsigned int* pixPtr = (unsigned int*)TargetFrameBuffer->BaseAddress;
	char* fontPtr = (char*)PSF1_Font->glyphBuffer + (chr * PSF1_Font->psf1_Header->charsize);

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