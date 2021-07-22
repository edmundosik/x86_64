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

KernelInfo InitializeKernel(BootInfo* bootInfo) {
    GDTDescriptor gdtDescriptor;
    gdtDescriptor.Size = sizeof(GDT) - 1;
    gdtDescriptor.Offset = (uint64_t)&DefaultGDT;
    LoadGDT(&gdtDescriptor);

    PrepareMemory(bootInfo);
    memset(bootInfo->framebuffer->BaseAddress, 0, bootInfo->framebuffer->BufferSize); // clear screen
    return kernelInfo;
}