#include "kernelUtil.h"

extern "C" void _start(BootInfo* bootInfo) {
	KernelInfo kernelInfo = InitializeKernel(bootInfo);
	PageTableManager* pageTableManager = kernelInfo.pageTableManager;
	BasicRenderer renderer = BasicRenderer(bootInfo->framebuffer, bootInfo->psf1_font);
	
	renderer.print("Kernel successfully initialized.");
	renderer.CursorPosition = {0, renderer.CursorPosition.Y + 16};

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
	
	while(true);
}