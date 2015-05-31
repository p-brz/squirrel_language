#ifndef TYPE_CHECKER_H
#define TYPE_CHECKER_H

#include <stdbool.h>
#include "squirrel_context.h"
#include "compiler_types.h"
#include "cast_rules.h"

bool checkCategoryEquals(SquirrelContext * sqContext, TypeCategory current, TypeCategory expected);

/** Testa se a função de nome 'functionName' pode ser convertida
    para o tipo de função 'functionTypename'*/
bool sq_can_coerce_function_to(SquirrelContext * ctx, 
    const char * functionName, const char * functionTypename);
    
/** Aplica as regras de compatibilidade de tipos para obter o tipo resultante.*/
type sq_getResultantType(SquirrelContext * ctx, type type1, type type2);

type sq_getArrayType(SquirrelContext * ctx, ExpressionList * arrayItems);
type sq_getArrayItemType(SquirrelContext * ctx, ExpressionList * arrayItems);

#endif