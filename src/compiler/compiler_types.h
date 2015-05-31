#ifndef COMPILER_HELPER_H
#define COMPILER_HELPER_H

#include <stdbool.h>
#include "arraylist.h"
#include "sq_metatype.h"
#include "squirrel_context.h"
/** ***************************************************************************************
 compiler_types.h
    - tipos utilizados nas regras do compilador yacc (e funções associadas a estes tipos)
  
*******************************************************************************************/

typedef enum {
    categ_unknown, //símbolo não existe
    categ_primitiveType, 
    categ_arrayType,
    categ_structType, 
    categ_structField, 
    categ_functionType, 
    categ_enumType, 
    categ_enumFieldValue,
    categ_function, 
    categ_variable, 
    categ_namespace}  Category;
    
typedef arraylist NameDeclList;
typedef arraylist ParamList;
typedef arraylist AttributeList;
typedef arraylist ExpressionList;
/** NameList é apenas uma abstração para um arraylist que contém nomes (char *) */
typedef arraylist NameList;

typedef enum {
                opcategory_invalid, 
                opcategory_arithmetic, 
                opcategory_bitwise, 
                opcategory_relational, 
                opcategory_logical} OperatorCategory;

typedef struct Member{
    Category category;
    char * name;//ID especifica deste membro
    char * tableKey; //Chave na tabela de símbolos
    struct Member * parent;
} Member;

typedef struct {
    char *type;
    char *expr;
    TypeCategory typeCategory;
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

const char * sq_categoryCString(Category category);

Member * sq_Member(const char * name, const char * tableKey, Category category, Member * parent);
void sq_destroyMember(Member * member);
char * sq_memberToString(const Member * member);
Expression * sq_memberToExpression(SquirrelContext * context, const Member * member);

Parameter * sq_Parameter(const char * typeName, const char * name, arraylist * modifiersList);
char * sq_ParameterToString(void * parameter);

NameDeclItem * sq_NameDeclItem(const char * name, Expression * expr);
char * sq_NameDeclToString(void * item);

AttributeDecl * sq_AttributeDecl(const char * typeName, NameList * namesList);
char * attributeListToString(AttributeList * attributeList);

Expression *sq_Expression( const char *type, const char *expr, TypeCategory typeCategory);
char *sq_exprToStr( Expression *expr );
char * sq_ExpressionStringConverter(void *);

IfStruct * sq_IfStruct(char * ifId, char * conditional_test, char * block_stmts);

NameList * sq_ForHeader(char * initStmt, char * testConditional, char * incrStmt);

OperatorCategory sq_getOperatorCategory(const char * operatorStr);
#endif
