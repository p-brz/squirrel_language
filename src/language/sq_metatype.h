#ifndef SQ_TYPE_EXPR_H
#define SQ_TYPE_EXPR_H

#include "sq_types.h"
#include "cast_rules.h"

typedef struct type{
    const char * typename;
    TypeCategory category;
    struct type * arrayOf;//Tipos array terão um apontador para o tipo gerador
} type;

type create_Type(const char * typename, TypeCategory category, type * arrayOf);

//Operadores
string typename(const type t);

boolean caststo(type t1, type t2);

boolean equals_type(type t1, type t2);
#endif
