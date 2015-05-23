#include "sq_types.h"
#include <string.h>

string create_String(const char * strValue){
    MemoryNode * memory = allocaMemory(sizeof(char), strlen(strValue));
    
    string str;
    str.memory = memory;
    str.cstr = (char *)strcpy(memory->memory, strValue);
    return str;
}
