#ifndef SQ_GENERATOR_H
#define SQ_GENERATOR_H

#include "squirrel_context.h"
#include "compiler_types.h"

char * gen_program(SquirrelContext * ctx, const char * declarationsList);
char * sq_genIfBlock(IfStruct * ifStruct);
char * sq_genIfElseBlock(IfStruct * ifStruct, const char * elseStmts);
char * sq_genWhile(SquirrelContext * sqContext, const char * conditional_test, const char * block_stmts);
char * sq_genDoWhile(SquirrelContext * sqContext, const char * block_stmts, const char * conditional_test);
#endif
