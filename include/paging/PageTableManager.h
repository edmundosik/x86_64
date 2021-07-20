#pragma once
#include <stdint.h>
#include "paging/paging.h"

class PageTableManager {
    public:
        PageTableManager(PageTable* PML4Address);
        PageTable* PML4;
        void MapMemory(void* virtualMemory, void* physicalMemory);
};