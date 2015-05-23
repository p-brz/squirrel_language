#ifndef SQ_MEMORY_H
#define SQ_MEMORY_H

#include <stdlib.h>

typedef struct MemoryNode{
    int refCount;
    void * memory;
} MemoryNode;

MemoryNode * putMemory(void * memory);

MemoryNode * allocaMemory(size_t elementSize, size_t numberOfElements);

void free_memory();

void sq_memory_setup();

#endif
