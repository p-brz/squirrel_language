#include "sq_metatype.h"
#include "cast_rules.h"
#include <string.h>

type create_Type(const char * typename, TypeCategory category, type * arrayOf){
    type t;
    t.typename = typename;
    t.arrayOf = arrayOf;
    return t;
}

string typename(const type t){
    return create_String(t.typename);
}

boolean caststo(type t1, type t2){
    return equals_type(t1, t2)
            || can_coerce_to(t1.typename, t1.category, t2.typename, t2.category)
            || can_force_coerce_to(t1.category, t2.category);    
}


boolean equals_type(type t1, type t2){
    if(t1.arrayOf == NULL && t2.arrayOf == NULL){
        //Tipos básicos (não array), compara nome
        return (strcmp(t1.typename, t2.typename) == 0);
    }
    else if(t1.arrayOf != NULL && t2.arrayOf != NULL){
        //Se ambos forem arrays, compara recursivamente o tipo desses arrays
        return equals_type(*t1.arrayOf, *t2.arrayOf);
    }
    //Um é array e outro não
    return false;
}
