#ifndef SQ_TYPES_H
#define SQ_TYPES_H

#include "sq_memory.h"
#include "sq_struct.h"
#include "sq_array.h"

//-------------------------- Types definition ----------------------------------
typedef struct{
    MemoryNode * memory;
    char * cstr;
} string;

string create_String(const char * strValue);

typedef int boolean;

#define true 1
#define false 0

//----------------------------- toCString --------------------------------------
const char * string_toCString(string stringValue);
const char * boolean_toCString(boolean booleanValue);
const char * int_toCString(int value);
const char * long_toCString(long longValue);
const char * float_toCString(float floatValue);

#endif
