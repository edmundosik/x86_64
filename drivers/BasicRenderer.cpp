#include <BasicRenderer.h>

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