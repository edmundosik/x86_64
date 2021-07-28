#include "kernelUtil.h"

extern "C" void _start(BootInfo* bootInfo) {
	KernelInfo kernelInfo = InitializeKernel(bootInfo);
	PageTableManager* pageTableManager = kernelInfo.pageTableManager;
	
	bgColor = 0xff4B5263;
	fgColor = 0xffC9CBD0;

	renderer->clearColor = bgColor;
	renderer->clear();
	renderer->color = fgColor;

	renderer->print("Kernel successfully initialized.");
	renderer->nextLine();
	
	Terminal tty;
	tty.prefixColor = 0xff028ee0;
	tty.textColor = 0xffC9CBD0;
	tty.prefix = (char*)"# ";
	tty.InitTTY();
	
	//renderer->cursor = true;

	// int* pagefault = (int*)0x80000000000;
	// *pagefault = 2;
	
	while(true) {
		
		tty.loop();
		//renderer->drawCursor();
		
	}

	while(true);
}