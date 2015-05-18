#ifndef COMPILER_HELPER_H
#define COMPILER_HELPER_H

#include "sqtypes.h"
#include <stdbool.h>
#include "arraylist.h"
/** ***************************************************************************************
 compiler_types.h
    - tipos utilizados nas regras do compilador yacc (e funções associadas a estes tipos)
  
*******************************************************************************************/

typedef arraylist NameDeclList;
typedef arraylist ParamList;
typedef arraylist AttributeList;

/** NameList é apenas uma abstração para um arraylist que contém nomes (char *) */
typedef arraylist NameList;

typedef struct {
    sq_type type;
    char *expr;
} Expression;


typedef struct{
    char * name;
    Expression * expr;
} NameDeclItem;

typedef struct  {
    char * type;
    char * name;
    bool isConst, isRef;
} Parameter;

typedef struct  {
    char * type;
    NameList * namesList;
} AttributeDecl;

Parameter * sq_Parameter(const char * typeName, const char * name, arraylist * modifiersList);
char * sq_ParameterToString(void * parameter);

NameDeclItem * sq_NameDeclItem(char * name, Expression * expr);
char * sq_NameDeclToString(void * item);

AttributeDecl * sq_AttributeDecl(char * typeName, NameList * namesList);

Expression *sq_Expression( sq_type type, const char *expr );
char *sq_exprToStr( Expression *expr );


#endif
