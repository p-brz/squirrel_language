#include "sq_types.h"

#include "string_helpers.h"

#include <string.h>


string empty_String(){
    string str;
    str.memory = NULL;
    str.cstr = NULL;
    return str;
}
string create_String(const char * strValue){
    MemoryNode * memory = allocaMemory(sizeof(char), strlen(strValue));
    
    string str;
    str.memory = memory;
    
    str.cstr = (char *)strcpy((char *)memory->memory, strValue);
    return str;
}

boolean equals_String(const string str1, const string str2){
    if(str1.cstr != NULL && str2.cstr != NULL){
        return strcmp(str1.cstr, str2.cstr) == 0;
    }
    else{
        return str1.cstr == NULL && str2.cstr == NULL;
    }
}

string typename(const type t){
    return create_String(t.typename);
}

const char * string_toCString(string stringValue){
    if(stringValue.memory == NULL || stringValue.cstr == NULL){
        return "null";
    }
    return stringValue.cstr;
}
const char * boolean_toCString(boolean booleanValue){
    return booleanValue ? "true" : "false";
}

const char * int_toCString(int value){
    char * cstring = intToString(value);
    putMemory(cstring);
    return cstring;
}

const char * long_toCString(long longValue){
    char * cstring = longToString(longValue);
    putMemory(cstring);
    return cstring;
}
const char * float_toCString(float floatValue){
    char * cstring = realToString(floatValue);
    putMemory(cstring);
    return cstring;
}
