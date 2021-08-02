#pragma once
#include <stdint.h>
#include <stddef.h>
#include "memory/paging/PageTableManager.h"
#include "memory/paging/PageFrameAllocator.h"

struct HeapSegHdr {
    size_t length;
    HeapSegHdr* next;
    HeapSegHdr* last;
    bool free;
    void CombineForward();
    void CombineBackward();
    HeapSegHdr* Split(size_t splitLength);
};

void InitHeap(void* headAddress, size_t pageCount);

void* malloc(size_t size);
void free(void* address);

void ExpandHeap(size_t length);