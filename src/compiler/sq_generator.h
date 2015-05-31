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
char * sq_genLenghtExpr(SquirrelContext * sqContext ,  Member * member);

char * sq_genPrint(SquirrelContext * sqContext, ExpressionList *list);
char * sq_genStruct(SquirrelContext * sqContext, char *structName, AttributeList * attributeDeclList);
char * sq_genFuncionType (SquirrelContext * sqContext, char * type ,char * id ,ParamList * param); 
char * sq_genTypeof(char *type);
char * sq_genEnum(SquirrelContext * sqContext, char * id ,char * id_list );


//OBS.: gerando código parcialmente
char * sq_genFunction(SquirrelContext * ctx, const char * returnType
, const char * functionName, ParamList * paramList, const char * functionBody);

#endif
