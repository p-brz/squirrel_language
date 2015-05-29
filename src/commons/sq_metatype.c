#include "sq_metatype.h"
#include "cast_rules.h"
#include <string.h>


const char * sq_typeCategoryCString(TypeCategory typeCategory){
    switch(typeCategory){
        case type_void:
            return "type_void";
        case type_array:
            return "type_array";
        case type_boolean:
            return "type_boolean";
        case type_enum:
            return "type_enum";
        case type_function:
            return "type_function";
        case type_integer:
            return "type_integer";
        case type_object:
            return "type_object";
        case type_real:
            return "type_real";
        case type_type:
            return "type_type";
        case type_string:
            return "type_string";
        case type_struct:
            return "type_struct";
        case type_invalid:
            return "type_invalid";
    }   
    return "";
}

type create_Type(const char * typename, TypeCategory category, type * arrayOf){
    type t;
    t.typename = typename;
    t.arrayOf = arrayOf;
    return t;
}

bool caststo(type t1, type t2){
    return equals_type(t1, t2)
            || can_coerce_to(t1.typename, t1.category, t2.typename, t2.category)
            || can_force_coerce_to(t1.category, t2.category);    
}


bool equals_type(type t1, type t2){
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
