#include <stddef.h>
#include "BasicRenderer.h"
#include "cstr.h"
#include "memory.h"
#include "Bitmap.h"
#include "paging/PageFrameAllocator.h"
#include "paging/PageMapIndexer.h"

struct BootInfo{
	FrameBuffer* framebuffer;
	PSF1_FONT* psf1_font;
	EFI_MEMORY_DESCRIPTOR* mMap;
	uint64_t mMapSize;
	uint64_t mMapDescSize;
};

extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;

extern "C" void _start(BootInfo* bootInfo) {
	BasicRenderer renderer = BasicRenderer(bootInfo->framebuffer, bootInfo->psf1_font);
		
	uint64_t mMapEntries = bootInfo->mMapSize / bootInfo->mMapDescSize;

	pageAllocator = PageFrameAllocator();
	pageAllocator.ReadEFIMemoryMap(bootInfo->mMap, bootInfo->mMapSize, bootInfo->mMapDescSize);

	uint64_t kernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;
	uint64_t kernelPages = (uint64_t)kernelSize / 4096 + 1;

	pageAllocator.LockPages(&_KernelStart, kernelPages);

	PageMapIndexer pageIndexer = PageMapIndexer(0x1000 * 52 + 0x50000 * 7);

	renderer.print("Free RAM: ");
	renderer.print(to_string(pageAllocator.GetFreeRAM() / 1048576));
	renderer.print(" MB");
	renderer.CursorPosition = {0, renderer.CursorPosition.Y + 16};

	renderer.print("Used RAM: ");
	renderer.print(to_string(pageAllocator.GetUsedRAM() / 1024));
	renderer.print(" KB");
	renderer.CursorPosition = {0, renderer.CursorPosition.Y + 16};

	renderer.print("Reserved RAM: ");
	renderer.print(to_string(pageAllocator.GetReservedRAM() / 1048576));
	renderer.print(" MB");
	renderer.CursorPosition = {0, renderer.CursorPosition.Y + 16};

	renderer.print(to_string(pageIndexer.P_i));
	renderer.print(" - ");
	renderer.print(to_string(pageIndexer.PT_i));
	renderer.print(" - ");
	renderer.print(to_string(pageIndexer.PD_i));
	renderer.print(" - ");
	renderer.print(to_string(pageIndexer.PDP_i));

	//for(;;);
	return;
}