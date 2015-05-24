#ifndef SQ_TYPES_H
#define SQ_TYPES_H

#include "sq_memory.h"
#include "sq_struct.h"
#include "sq_array.h"

//----------------------------- String ----------------------------------------
typedef struct{
    MemoryNode * memory;
    char * cstr;
} string;

string create_String(const char * strValue);

//----------------------------- Boolean ----------------------------------------
typedef int boolean;

#define true 1
#define false 0

char * boolean_toCString(boolean booleanValue);
char * long_toCString(long longValue);
char * float_toCString(float floatValue);

#endif
