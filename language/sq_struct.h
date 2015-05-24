#ifndef SQ_STRUCT_H
#define SQ_STRUCT_H

#include "sq_memory.h"

typedef struct StructVar{
    MemoryNode * memory;
    void * structValue;
} StructVar;

StructVar create_Struct(void * structValue);
StructVar * incrRefCount_StructVar(void * value);
StructVar * decrRefCount_StructVar(void * value);

#define getStructMember(structType, structVar, name)\
    (((structType *)structVar.structValue)->name)
    
#endif
