#include "kernelUtil.h"

KernelInfo kernelInfo;

uint32_t bgColor;
uint32_t fgColor;

ACPI::SDTHeader* xsdt;
ACPI::MCFGHeader* mcfg;

void PrepareMemory(BootInfo* bootInfo) {
    uint64_t mMapEntries = bootInfo->mMapSize / bootInfo->mMapDescSize;

	PageAllocator = PageFrameAllocator();
	PageAllocator.ReadEFIMemoryMap(bootInfo->mMap, bootInfo->mMapSize, bootInfo->mMapDescSize);

	uint64_t kernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;
	uint64_t kernelPages = (uint64_t)kernelSize / 4096 + 1;

	PageAllocator.LockPages(&_KernelStart, kernelPages);

	PageTable* PML4 = (PageTable*)PageAllocator.RequestPage();
	memset(PML4, 0, 0x1000);

	gPageTableManager = PageTableManager(PML4);
	
	for(uint64_t i = 0; i < GetMemorySize(bootInfo->mMap, mMapEntries, bootInfo->mMapDescSize); i += 0x1000) {
		gPageTableManager.MapMemory((void*)i, (void*)i);
	}
	
	uint64_t fbBase = (uint64_t)bootInfo->framebuffer->BaseAddress;
	uint64_t fbSize = (uint64_t)bootInfo->framebuffer->BufferSize + 0x1000;
	PageAllocator.LockPages((void*)fbBase, fbSize / 0x1000 + 1);
	for(uint64_t i = fbBase; i < fbBase + fbSize; i += 4096) {
		gPageTableManager.MapMemory((void*)i, (void*)i);
	}

	asm("mov %0, %%cr3" : : "r" (PML4));

	kernelInfo.pageTableManager = &gPageTableManager;
}

IDTR idtr;
void SetIDTGate(void* handler, uint8_t entryOffset, uint8_t typeAttr, uint8_t selector) {
	IDTDescEntry* interrupt = (IDTDescEntry*)(idtr.Offset + entryOffset * sizeof(IDTDescEntry));
	interrupt->SetOffset((uint64_t)handler);
	interrupt->type_attr = typeAttr;
	interrupt->selector = selector;
}

void PrepareInterrupts() {
	idtr.Limit = 0x0fff;
	idtr.Offset = (uint64_t)PageAllocator.RequestPage();

	SetIDTGate((void*)PageFault_Handler, 0xE, IDT_TA_InterruptGate, 0x08);
	SetIDTGate((void*)DoubleFault_Handler, 0x8, IDT_TA_InterruptGate, 0x08);
	SetIDTGate((void*)GPFault_Handler, 0xD, IDT_TA_InterruptGate, 0x08);
	SetIDTGate((void*)KeyboardInt_Handler, 0x21, IDT_TA_InterruptGate, 0x08);
	SetIDTGate((void*)MouseInt_Handler, 0x2C, IDT_TA_InterruptGate, 0x08);
	SetIDTGate((void*)PITInt_Handler, 0x20, IDT_TA_InterruptGate, 0x08);

	asm("lidt %0" : : "m" (idtr));

	RemapPIC();	
}

void PrepareACPI(BootInfo* bootInfo) {
	xsdt = (ACPI::SDTHeader*)(bootInfo->rsdp->XSDTAddress);
	mcfg = (ACPI::MCFGHeader*)ACPI::FindTable(xsdt, (char*)"MCFG");

	//PCI::EnumeratePCI(mcfg);
}

BasicRenderer r = BasicRenderer(NULL, NULL);
KernelInfo InitializeKernel(BootInfo* bootInfo) {
	r = BasicRenderer(bootInfo->framebuffer, bootInfo->psf1_font);
	renderer = &r;

    PrepareMemory(bootInfo);
    memset(bootInfo->framebuffer->BaseAddress, 0, bootInfo->framebuffer->BufferSize); // clear screen

	bgColor = 0xff4B5263;
	fgColor = 0xffC9CBD0;

	renderer->clearColor = bgColor;
	renderer->clear();
	renderer->color = fgColor;

	GDTDescriptor gdtDescriptor;
    gdtDescriptor.Size = sizeof(GDT) - 1;
    gdtDescriptor.Offset = (uint64_t)&DefaultGDT;
    LoadGDT(&gdtDescriptor);
	renderer->print("[ ");
	renderer->color = 0xff00ff00;
	renderer->print("OK");
	renderer->color = fgColor;
	renderer->print(" ] ");
	renderer->print("GDT loaded");
	renderer->nextLine();

	InitHeap((void*)0x0000100000000000, 0x10);
	renderer->print("[ ");
	renderer->color = 0xff00ff00;
	renderer->print("OK");
	renderer->color = fgColor;
	renderer->print(" ] ");
	renderer->print("Heap initialized");
	renderer->nextLine();

	PrepareInterrupts();
	renderer->print("[ ");
	renderer->color = 0xff00ff00;
	renderer->print("OK");
	renderer->color = fgColor;
	renderer->print(" ] ");
	renderer->print("Interrupts initialized");
	renderer->nextLine();

	InitPS2Mouse();
	renderer->print("[ ");
	renderer->color = 0xff00ff00;
	renderer->print("OK");
	renderer->color = fgColor;
	renderer->print(" ] ");
	renderer->print("PS/2 Mouse initialized");
	renderer->nextLine();

	PrepareACPI(bootInfo);
	renderer->print("[ ");
	renderer->color = 0xff00ff00;
	renderer->print("OK");
	renderer->color = fgColor;
	renderer->print(" ] ");
	renderer->print("ACPI initialized");
	renderer->nextLine();

	outb(PIC1_DATA, 0b11111000);
	outb(PIC2_DATA, 0b11101111);
	renderer->print("[ ");
	renderer->color = 0xff00ff00;
	renderer->print("OK");
	renderer->color = fgColor;
	renderer->print(" ] ");
	renderer->print("Unmasked interrupts");
	renderer->nextLine();

	PIT::SetDivisor(65535);
	renderer->print("[ ");
	renderer->color = 0xff00ff00;
	renderer->print("OK");
	renderer->color = fgColor;
	renderer->print(" ] ");
	renderer->print("PIT initialized");
	renderer->nextLine();

	if(InitSerial() == 0) {
		renderer->print("[ ");
		renderer->color = 0xff00ff00;
		renderer->print("OK");
		renderer->color = fgColor;
		renderer->print(" ] ");
		renderer->print("Serial initialized");
		renderer->nextLine();
	} else {
		renderer->print("[ ");
		renderer->color = 0xffff0000;
		renderer->print("ERROR");
		renderer->color = fgColor;
		renderer->print(" ] ");
		renderer->print("Serial initialization failed");
		renderer->nextLine();
	}

	asm("sti");

    return kernelInfo;
}