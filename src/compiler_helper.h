#ifndef COMPILER_HELPER_H
#define COMPILER_HELPER_H

#include "sqtypes.h"

/** ***************************************************************************************
 compiler_helper.h
    - tipos utilizados nas regras do compilador yacc (e funções associadas a estes tipos)
  
*******************************************************************************************/

typedef struct {
    sq_type type;
    char *expr;
} Expression;


typedef struct{
    char * name;
    Expression * expr;
} NameDeclItem;

NameDeclItem * sq_NameDeclItem(char * name, Expression * expr);

char * sq_NameDeclToString(void * item);

Expression *sq_Expression( sq_type type, const char *expr );

char *sq_exprToStr( Expression *expr );

#endif
