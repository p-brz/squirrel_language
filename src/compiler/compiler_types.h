#ifndef COMPILER_HELPER_H
#define COMPILER_HELPER_H

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
    char *type;
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

typedef struct {
    char * ifId;
    char * conditional_test;
    char * block_stmts;
} IfStruct;


Parameter * sq_Parameter(const char * typeName, const char * name, arraylist * modifiersList);
char * sq_ParameterToString(void * parameter);

NameDeclItem * sq_NameDeclItem(const char * name, Expression * expr);
char * sq_NameDeclToString(void * item);

AttributeDecl * sq_AttributeDecl(const char * typeName, NameList * namesList);
char * attributeListToString(AttributeList * attributeList);

Expression *sq_Expression( const char *type, const char *expr );
char *sq_exprToStr( Expression *expr );

IfStruct * sq_IfStruct(char * ifId, char * conditional_test, char * block_stmts);

NameList * sq_ForHeader(char * initStmt, char * testConditional, char * incrStmt);
#endif
