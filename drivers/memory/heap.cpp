#include "memory/heap.h"

void* heapStart;
void* heapEnd;
HeapSegHdr* LastHdr;

void InitHeap(void* heapAddress, size_t pageCount) {
    void* pos = heapAddress;

    for(size_t i = 0; i < pageCount; i ++) {
        gPageTableManager.MapMemory(pos, PageAllocator.RequestPage());
        pos = (void*)((size_t)pos + 0x1000);
    }

    size_t heapLength = pageCount * 0x1000;
    

    heapStart = heapAddress;
    heapEnd = (void*)((size_t)heapStart + heapLength);
    HeapSegHdr* startSeg = (HeapSegHdr*)heapAddress;
    startSeg->length = heapLength - sizeof(HeapSegHdr);
    startSeg->next = NULL;
    startSeg->last = NULL;
    startSeg->free = true;
    LastHdr = startSeg;
}


void* malloc(size_t size) {
    if(size % 0x10 > 0) {
        size -= (size % 0x10);
        size += 0x10;
    }

    if(size == 0)
        return NULL;

    HeapSegHdr* currentSeg = (HeapSegHdr*)heapStart;
    while(true) {
        if(currentSeg->free) {
            if(currentSeg->length > size) {
                currentSeg->Split(size);
                currentSeg->free = false;
                return (void*)((uint64_t)currentSeg + sizeof(HeapSegHdr));
            }

            if(currentSeg->length > size) {
                currentSeg->free = false;
                return (void*)((uint64_t)currentSeg + sizeof(HeapSegHdr));
            }
        }

        if(currentSeg->next == NULL)
            break;
        currentSeg = currentSeg->next;
    }
    ExpandHeap(size);
    return malloc(size);
}

void free(void* address) {
    HeapSegHdr* seg = (HeapSegHdr*)address - 1;
    seg->free = true;
    seg->CombineForward();
    seg->CombineBackward();
}

HeapSegHdr* HeapSegHdr::Split(size_t splitLength) {
    if(splitLength < 0x10)
        return NULL;
    int64_t splitSegLength = length - splitLength - (sizeof(HeapSegHdr));
    if(splitSegLength < 0x10)
        return NULL;
    
    HeapSegHdr* newSplitHdr = (HeapSegHdr*)((size_t)this + splitLength + sizeof(HeapSegHdr));
    next->last = newSplitHdr;
    newSplitHdr->next = next;
    next = newSplitHdr;
    newSplitHdr->last = this;
    newSplitHdr->length = splitSegLength;
    newSplitHdr->free = free;
    length = splitLength;

    if(LastHdr == this)
        LastHdr = newSplitHdr;
    return newSplitHdr;
}

void ExpandHeap(size_t length) {
    if(length % 0x1000) {
        length -= length % 0x1000;
        length += 0x1000;
    }

    size_t pageCount = length / 0x1000;
    HeapSegHdr* newSeg = (HeapSegHdr*)heapEnd;

    for(size_t i = 0; i < pageCount; i++) {
        gPageTableManager.MapMemory(heapEnd, PageAllocator.RequestPage());
        heapEnd = (void*)((size_t)heapEnd + 0x1000);
    }

    newSeg->free = true;
    newSeg->last = LastHdr;
    LastHdr->next = newSeg;
    LastHdr = newSeg;
    newSeg->next = NULL;
    newSeg->length = length - sizeof(HeapSegHdr);
    newSeg->CombineBackward();
}

void HeapSegHdr::CombineForward() {
    if(next == NULL)
        return;
    if(!next->free)
        return;
    
    if(next == LastHdr)
        LastHdr == this;
    
    if(next->next != NULL)
        next->next->last = this;

    length = length + next->length + sizeof(HeapSegHdr);
}

void HeapSegHdr::CombineBackward() {
    if(last != NULL && last->free)
        last->CombineForward();
}