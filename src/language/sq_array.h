#ifndef SQ_ARRAY_H
#define SQ_ARRAY_H

#include "sq_memory.h"

typedef struct Array{
    MemoryNode * memory;
    void * startPos;
    size_t length;
    size_t itemSize;
} Array;

Array empty_Array();

Array create_Array(size_t memberSize, size_t numberOfElements, const void * arrValues);

//TODO: incluir checagem de faixa
#define getArrayItem(type, array, index)\
    (((type *)array.startPos)[index])

void * getItemPointer(const Array * array, unsigned index);    

typedef void (* MemoryIncrementer)(void *);
typedef void (* MemoryDecrementer)(void *);

void incrRefCount_Array(Array array, MemoryIncrementer incrementer);
void decrRefCount_Array(Array array, MemoryDecrementer decrementer);

#endif
