#ifndef TYPE_CHECKER_H
#define TYPE_CHECKER_H

#include <stdbool.h>
#include "squirrel_context.h"
#include "compiler_types.h"
#include "cast_rules.h"


/** Checa se tipo de expr1 pode ser convertido através de coerção para tipo de expr2. */
bool sq_checkExpressionCoercion(SquirrelContext * sqContext, Expression * expr1, Expression * expr2);
bool sq_checkTypeCoercion(SquirrelContext * sqContext, type t1, type t2);
bool sq_checkExpressionIsCallable(SquirrelContext * sqContext, Expression * expr);
bool sq_checkVariablesDeclCoercion(SquirrelContext * sqContext, const char * typeName, NameDeclList * nameDeclList);
                                                                    
bool sq_checkExistMember(SquirrelContext * sqContext, Member * member);
                                                        
bool checkCategoryEquals(SquirrelContext * sqContext, TypeCategory current, TypeCategory expected);

/** Checa se cast de expr para typeName pode ser feito.*/
bool checkCastRule(SquirrelContext * sqContext
    , const char * typeName, TypeCategory typeCategory, Expression * expr);

/** Testa se a função de nome 'functionName' pode ser convertida
    para o tipo de função 'functionTypename'*/
bool sq_can_coerce_function_to(SquirrelContext * ctx, 
    const char * functionName, const char * functionTypename);
    
/** Aplica as regras de compatibilidade de tipos para obter o tipo resultante.*/
type sq_getResultantType(SquirrelContext * ctx, type type1, type type2);

type sq_getArrayType(SquirrelContext * ctx, ExpressionList * arrayItems);
type sq_getArrayItemType(SquirrelContext * ctx, ExpressionList * arrayItems);

type sq_getBinaryExpressionType(SquirrelContext * sqContext, const char * operator, Expression * expr1, Expression * expr2);
type sq_getResultingTypeUnary(SquirrelContext * sqContext, OperatorCategory operatorCategory, Expression * expr);
type sq_getUnaryExpressionType(SquirrelContext * sqContext, const char * operator , Expression * expr);



#endif