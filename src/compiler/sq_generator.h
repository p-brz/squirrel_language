#ifndef SQ_GENERATOR_H
#define SQ_GENERATOR_H

#include "squirrel_context.h"
#include "compiler_types.h"

char * gen_program(SquirrelContext * ctx, const char * declarationsList);
char * sq_genIfBlock(IfStruct * ifStruct);
char * sq_genIfElseBlock(IfStruct * ifStruct, const char * elseStmts);
char * sq_genWhile(SquirrelContext * sqContext, const char * conditional_test, const char * block_stmts);
char * sq_genDoWhile(SquirrelContext * sqContext, const char * block_stmts, const char * conditional_test);
char * sq_genForBlock(SquirrelContext * sqContext, NameList * forStatements, const char * block_stmts);

char * sq_genCoercion(SquirrelContext * sqContext, Expression * expr, const char * typename);
/** Gera string a ser inclusa no código fonte substituindo typeName.
    Isto permite traduzir tipos array e alguns literais.
*/
char * sq_translateTypeName(SquirrelContext * sqContext, const char * typeName);

char * sq_genLenghtExpr(SquirrelContext * sqContext ,  Member * member);
char * sq_genPrint(SquirrelContext * sqContext, ExpressionList *list);
char * sq_genStruct(SquirrelContext * sqContext, char *structName, AttributeList * attributeDeclList);
char * sq_genFunctionType (SquirrelContext * sqContext, char * type ,char * id ,ParamList * param); 
char * sq_genTypeof(const char *type);
char * sq_genTypename(SquirrelContext * sqContext, Expression * typeOrExpr);
char * sq_genCaststTo(SquirrelContext * sqContext, Expression * typeOrExpr1, Expression * typeOrExpr2);
char * sq_genEnum(SquirrelContext * sqContext, char * id , arraylist * id_list );
char * sq_genCreateEmptyArray();
char * sq_genSizedArray(SquirrelContext * sqContext, const char * itemTypeName, Expression * sizeExpr);
char * sq_genArrayLiteralCreator(SquirrelContext * sqContext, type itemType, ExpressionList * exprList);
char * sq_genIndexAccess(SquirrelContext * sqContext, Expression *arrayExpr, Expression *IndexExpr);
char * sq_genCastExpr(SquirrelContext * sqContext, const char * typeName, TypeCategory typeCategory, Expression * expr);

//OBS.: gerando código parcialmente
char * sq_genFunction(SquirrelContext * ctx, const char * returnType
, const char * functionName, ParamList * paramList, const char * functionBody);

#endif
