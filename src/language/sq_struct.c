#include "sq_struct.h"

StructVar create_Struct(void * structValue){
    MemoryNode * memNode = structValue != NULL ? putMemory(structValue) : NULL;
    
    StructVar structVar;
    structVar.memory = memNode;
    structVar.structValue = memNode != NULL ? memNode->memory : NULL;
    
    return structVar;
}
StructVar * incrRefCount_StructVar(void * value){
    StructVar * var = (StructVar *)value;
    incrRefCount(var->memory);
    return var;
}
StructVar * decrRefCount_StructVar(void * value){
    StructVar * var = (StructVar *)value;
    decrRefCount(var->memory);
    return var;
}
