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
//libera toda a memória alocada
void free_All();

void incrRefCount(MemoryNode * memNode);
void decrRefCount(MemoryNode * memNode);

void sq_memory_setup();

/** Função para debugar memória atual*/
void printDebugMemory();

#endif
