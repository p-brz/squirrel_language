#include "sq_cast.h"
#include "stdlib.h"

//Numbers to string
string integer_to_string  (long value){
    return create_String(long_toCString(value));
}
string real_to_string(double value){
    return create_String(real_toCString(value));
}
string boolean_to_string(boolean value){
    return create_String(boolean_toCString(value));
}
//String to numbers
#define cast_string_to_integer(type)\
    type string_to_##type(const string value){\
        return (type)(value.cstr != NULL ? strtol(value.cstr,NULL,0) : 0);\
    }
#define cast_string_to_real(type)\
    type string_to_##type(const string value){\
        return (type)(value.cstr != NULL ? atof(value.cstr) : 0);\
    }
    
cast_string_to_integer(byte);
cast_string_to_integer(short);
cast_string_to_integer(int);
cast_string_to_integer(long);
cast_string_to_real(float);
cast_string_to_real(double);

boolean string_to_boolean(const string value){
    if(value.cstr != NULL && strEquals(value.cstr, "true")){
        return true;
    }
    return false;
}