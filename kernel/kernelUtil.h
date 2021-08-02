#pragma once
#include <stddef.h>
#include "gfx/BasicRenderer.h"
#include "cstr.h"
#include "memory/memory.h"
#include "memory/Bitmap.h"
#include "memory/heap.h"
#include "memory/paging/PageFrameAllocator.h"
#include "memory/paging/PageMapIndexer.h"
#include "memory/paging/PageTableManager.h"
#include "memory/paging/paging.h"
#include "gdt/gdt.h"
#include "interrupts/IDT.h"
#include "interrupts/interrupts.h"
#include "io.h"
#include "input/mouse.h"
#include "console/console.h"
#include "pci/acpi.h"
#include "pci/pci.h"
#include "scheduling/pit/pit.h"
#include "date.h"
#include "serial/serial.h"
#include "serial/terminal.h"

struct BootInfo{
	FrameBuffer* framebuffer;
	PSF1_FONT* psf1_font;
	EFI_MEMORY_DESCRIPTOR* mMap;
	uint64_t mMapSize;
	uint64_t mMapDescSize;
	ACPI::RSDP2* rsdp;
};

extern uint32_t bgColor;
extern uint32_t fgColor;

extern ACPI::SDTHeader* xsdt;
extern ACPI::MCFGHeader* mcfg;

extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;

struct KernelInfo {
    PageTableManager* pageTableManager;
};

KernelInfo InitializeKernel(BootInfo* bootInfo);