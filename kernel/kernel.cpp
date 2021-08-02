#include "kernelUtil.h"

extern "C" void _start(BootInfo* bootInfo) {
	KernelInfo kernelInfo = InitializeKernel(bootInfo);
	PageTableManager* pageTableManager = kernelInfo.pageTableManager;

	renderer->print("Kernel successfully initialized.");
	renderer->nextLine();
	
	// Console cmd;
	// cmd.prefixColor = 0xff028ee0;
	// cmd.textColor = 0xffC9CBD0;
	// cmd.prefix = (char*)"# ";
	// cmd.InitCMD();
	
	//renderer->cursor = true;

	// int* pagefault = (int*)0x80000000000;
	// *pagefault = 2;
	
	Terminal term;

	term.print("Test");

	while(true) {
		//cmd.loop();

		renderer->putChar(SerialRead());
		PIT::Sleep(50);
	}

	while(true);
}