#include "sq_metatype.h"

type create_Type(const char * typename, type * arrayOf){
    type t;
    t.typename = typename;
    t.arrayOf = arrayOf;
    return t;
}

string typename(const type t){
    return create_String(t.typename);
}
