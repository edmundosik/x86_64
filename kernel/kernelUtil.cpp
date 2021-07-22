#include "kernelUtil.h"

KernelInfo kernelInfo;
PageTableManager pageTableManager = NULL;

void PrepareMemory(BootInfo* bootInfo) {
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
	PageAllocator.LockPages((void*)fbBase, fbSize / 0x1000 + 1);
	for(uint64_t i = fbBase; i < fbBase + fbSize; i += 4096) {
		pageTableManager.MapMemory((void*)i, (void*)i);
	}

	asm("mov %0, %%cr3" : : "r" (PML4));

	kernelInfo.pageTableManager = &pageTableManager;
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

	asm("lidt %0" : : "m" (idtr));

	RemapPIC();	
}

BasicRenderer r = BasicRenderer(NULL, NULL);
KernelInfo InitializeKernel(BootInfo* bootInfo) {
	r = BasicRenderer(bootInfo->framebuffer, bootInfo->psf1_font);
	renderer = &r;

    GDTDescriptor gdtDescriptor;
    gdtDescriptor.Size = sizeof(GDT) - 1;
    gdtDescriptor.Offset = (uint64_t)&DefaultGDT;
    LoadGDT(&gdtDescriptor);

    PrepareMemory(bootInfo);
    memset(bootInfo->framebuffer->BaseAddress, 0, bootInfo->framebuffer->BufferSize); // clear screen
	PrepareInterrupts();
	InitPS2Mouse();

	outb(PIC1_DATA, 0b11111001);
	outb(PIC2_DATA, 0b11101111);

	asm("sti");

    return kernelInfo;
}