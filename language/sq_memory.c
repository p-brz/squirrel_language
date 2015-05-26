#include "sq_memory.h"
#include "arraylist.h"

arraylist * memorylist;

void sq_memory_setup(){
    memorylist = arraylist_create();
}

MemoryNode * putMemory(void * memory){
    MemoryNode * node = (MemoryNode *)malloc(sizeof(MemoryNode));
    node->refCount = 0;
    node->memory = memory;

    arraylist_add(memorylist, node);
    return node;
}

MemoryNode * allocaMemory(size_t elementSize, size_t numberOfElements){
    void * memory = calloc(numberOfElements, elementSize);
    return putMemory(memory);
}

void free_memory(){
    int i;
    void * value;
    arraylist_iterate(memorylist, i, value){
        MemoryNode * node = (MemoryNode *)value;
        if(node->refCount == 0){
            free(node->memory);
            arraylist_remove(memorylist, i);
            
            i--;//reduz contador em 1 para não pular item (já que lista foi reduzida)
        }
    }
}