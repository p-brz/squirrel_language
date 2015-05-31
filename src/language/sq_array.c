#include "sq_array.h"
    
#include <string.h>

Array empty_Array(){
    Array arr;
    memset(&arr, 0, sizeof(Array));
    return arr;
}

Array create_Array(size_t memberSize, size_t numberOfElements, const void * arrValues){
    MemoryNode * node = allocaMemory(memberSize, numberOfElements);
    
    Array arr;
    arr.length   = numberOfElements;
    arr.itemSize = memberSize;
    arr.memory   = node;
    arr.startPos = node->memory;
    
    //Copia valores em arrValues para o array
    if(arrValues != NULL){
        memcpy(arr.startPos, arrValues, memberSize * numberOfElements);
    }
    else{//Zera memória (talvez devesse realizar inicialização mais elaborada)
        memset(arr.startPos, 0, memberSize * numberOfElements);
    }
    
    return arr;
}

void * getItemPointer(const Array * array, unsigned index){
    if(array == NULL){
        return NULL;
    }
    return array->startPos + (index * array->itemSize);
}

void incrRefCount_Array(Array array, MemoryIncrementer incrementer){
    incrRefCount(array.memory);
    
    if(incrementer != NULL){
        unsigned i=0;
        for(; i < array.length; ++i){
            incrementer(getItemPointer(&array, i));    
        }
    }
}
void decrRefCount_Array(Array array, MemoryDecrementer decrementer){
    decrRefCount(array.memory);
    
    if(decrementer != NULL){
        unsigned i=0;
        for(; i < array.length; ++i){
            decrementer(getItemPointer(&array, i));    
        }
    }
}
