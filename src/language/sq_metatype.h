#ifndef SQ_TYPE_EXPR_H
#define SQ_TYPE_EXPR_H

#include "sq_types.h"

typedef struct type{
    const char * typename;
    struct type * arrayOf;//Tipos array terão um apontador para o tipo gerador
} type;

type create_Type(const char * typename, type * arrayOf);

string typename(const type t);

#endif
