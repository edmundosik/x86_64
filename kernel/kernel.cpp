#include "kernelUtil.h"

extern "C" void _start(BootInfo* bootInfo) {
	KernelInfo kernelInfo = InitializeKernel(bootInfo);
	PageTableManager* pageTableManager = kernelInfo.pageTableManager;
	
	renderer->clearColor = 0xff4B5263;
	renderer->clear();
	renderer->color = 0xffC9CBD0;

	renderer->print("Kernel successfully initialized.");
	renderer->nextLine();

	renderer->print("Free RAM: ");
	renderer->print(to_string(PageAllocator.GetFreeRAM() / 1048576));
	renderer->print(" MB");
	renderer->nextLine();

	renderer->print("Used RAM: ");
	renderer->print(to_string(PageAllocator.GetUsedRAM() / 1024));
	renderer->print(" KB");
	renderer->nextLine();

	renderer->print("Reserved RAM: ");
	renderer->print(to_string(PageAllocator.GetReservedRAM() / 1048576));
	renderer->print(" MB");
	renderer->nextLine();

	// int* pagefault = (int*)0x80000000000;
	// *pagefault = 2;
	
	while(true) {
		ProccessMousePacket();
	}

	while(true);
}