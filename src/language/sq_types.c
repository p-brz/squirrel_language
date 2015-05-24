#include "sq_types.h"

#include "string_helpers.h"

#include <string.h>

string create_String(const char * strValue){
    MemoryNode * memory = allocaMemory(sizeof(char), strlen(strValue));
    
    string str;
    str.memory = memory;
    
    str.cstr = (char *)strcpy((char *)memory->memory, strValue);
    return str;
}


char * boolean_toCString(boolean booleanValue){
    char * cstring = booleanValue ? cpyString("true") : cpyString("false");
    putMemory(cstring);
    return cstring;
}

char * long_toCString(long longValue){
    char * cstring = longToString(longValue);
    putMemory(cstring);
    return cstring;
}
char * float_toCString(float floatValue){
    char * cstring = realToString(floatValue);
    putMemory(cstring);
    return cstring;
}
