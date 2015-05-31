#ifndef SQ_TYPE_EXPR_H
#define SQ_TYPE_EXPR_H

#include <stdbool.h>

typedef enum {type_invalid 
                , type_typeliteral //Isto é utilizado para encapsular um tipo em uma expressão
                , type_void, type_type, type_integer, type_real, type_boolean
                , type_string, type_object, type_enum, type_array, type_function
                , type_struct} TypeCategory;

const char * sq_typeCategoryCString(TypeCategory typeCategory);

typedef struct type{
    const char * typename;
    TypeCategory category;
    struct type * arrayOf;//Tipos array terão um apontador para o tipo gerador
} type;

type create_Type(const char * typename, TypeCategory category, type * arrayOf);

//Operadores
//Movido typename p/ sq_types.h
//string typename(const type t);

bool caststo(type t1, type t2);

bool equals_type(type t1, type t2);
#endif
