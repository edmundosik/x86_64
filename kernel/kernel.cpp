#include <stddef.h>
#include "BasicRenderer.h"
#include "cstr.h"
#include "memory.h"
#include "Bitmap.h"
#include "paging/PageFrameAllocator.h"
#include "paging/PageMapIndexer.h"
#include "paging/PageTableManager.h"
#include "paging/paging.h"

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

	PageAllocator = PageFrameAllocator();
	PageAllocator.ReadEFIMemoryMap(bootInfo->mMap, bootInfo->mMapSize, bootInfo->mMapDescSize);

	uint64_t kernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;
	uint64_t kernelPages = (uint64_t)kernelSize / 4096 + 1;

	PageAllocator.LockPages(&_KernelStart, kernelPages);

	PageTable* PML4 = (PageTable*)PageAllocator.RequestPage();
	memset(PML4, 0, 0x1000);

	PageTableManager pageTableManager = PageTableManager(PML4);

	for(uint64_t i = 0; i < GetMemorySize(bootInfo->mMap, mMapEntries, bootInfo->mMapDescSize); i += 0x1000) {
		pageTableManager.MapMemory((void*)i, (void*)i);
	}

	uint64_t fbBase = (uint64_t)bootInfo->framebuffer->BaseAddress;
	uint64_t fbSize = (uint64_t)bootInfo->framebuffer->BufferSize + 0x1000;
	for(uint64_t i = fbBase; i < fbBase + fbSize; i += 4096) {
		pageTableManager.MapMemory((void*)i, (void*)i);
	}

	asm("mov %0, %%cr3" : : "r" (PML4));

	renderer.print("Free RAM: ");
	renderer.print(to_string(PageAllocator.GetFreeRAM() / 1048576));
	renderer.print(" MB");
	renderer.CursorPosition = {0, renderer.CursorPosition.Y + 16};

	renderer.print("Used RAM: ");
	renderer.print(to_string(PageAllocator.GetUsedRAM() / 1024));
	renderer.print(" KB");
	renderer.CursorPosition = {0, renderer.CursorPosition.Y + 16};

	renderer.print("Reserved RAM: ");
	renderer.print(to_string(PageAllocator.GetReservedRAM() / 1048576));
	renderer.print(" MB");
	renderer.CursorPosition = {0, renderer.CursorPosition.Y + 16};

	//for(;;);
	return;
}