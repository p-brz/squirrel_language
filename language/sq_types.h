#ifndef SQ_TYPES_H
#define SQ_TYPES_H

#include "sq_memory.h"

typedef struct{
    MemoryNode * memory;
    char * cstr;
} string;

string create_String(const char * strValue);

#endif
