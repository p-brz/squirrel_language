#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

/** ***************************************************************************************
 symboltable.h - tipos e funções para manipulação da tabela de símbolos
  
*******************************************************************************************/

#include "arraylist.h"
#include "hashtable.h"
#include "compiler_types.h"
#include "squirrel_context.h"

#include <stdbool.h> //inclui bool em C99

typedef enum {
    categ_primitiveType, 
    categ_structType, 
    categ_functionType, 
    categ_enumType, 
    categ_function, 
    categ_variable, 
    categ_namespace}  Category;

/* Forward-declaration de TableRow
    - como TableRow inclui os campos <Categoria>Row, ele não pode ser declarado antes dos campos
    - por sua vez, alguns campos (como VariableRow) usam ponteiros para TableRow
    - uma "Forward-declaration" permite resolver isto 
*/
typedef struct TableRow TableRow;

typedef struct{
    arraylist * fields; //Cada item da lista é um FieldValue
} StructTypeRow;

typedef struct{
    arraylist * identifiers;// Cada identificador do enum é uma string

} EnumTypeRow;

//Esta struct será utilizada para tipos de função e funções
typedef struct {
    TableRow *returnType; //TODO: substituir por typename
    arraylist * parameters; //Cada item da lista é um ParamValue
} FunctionRow;

typedef struct {
    TableRow * type; //TODO: substituir por typename
    bool isConst;
    //?escopo
}VariableRow;

//typedef struct NamespaceRow{
//    struct NamespaceRow * parent;
//} NamespaceRow;

typedef struct  {
    TableRow *type; //TODO: substituir por typename
    bool isConst, isRef;
} ParamValue;

typedef struct  {
    char * type;
    char * name;
} FieldValue;

typedef union {
    StructTypeRow   structValue;
    EnumTypeRow     enumValue;
    FunctionRow     functionValue;
    VariableRow     variableValue;
//    NamespaceRow    namespaceValue;
} TableRowValue;

typedef struct TableRow{
    char * name; //mesmo ponteiro da chave (permite acessar nome a partir de referência à TableRow)
    Category category;
    TableRowValue value;
} TableRow;

/** Insere na tabela de símbolos uma lista de nomes de variáveis (nameDeclList), 
    declaradas com um mesmo tipo (type)*/
void sq_declareVariables(SquirrelContext * sqContext, const char * typeName, arraylist * nameDeclList);

/** Similar a sq_declareVariables, mas cria variáveis são marcadas const isConst = true*/
void sq_declareConstants(SquirrelContext * sqContext, const char * typeName, arraylist * nameDeclList);

/** Declara função e inicia escopo.
*/
void sq_startFunction(SquirrelContext * sqContext, const char * returnType, const char * functionName, ParamList * parameters);

/** Insere na tabela de símbolos uma função com tipo de retorno 'returnType' e nome 'functionName'.

    @param paremeters - uma lista de ponteiros para Parameter (ver compiler_helper.h)
*/
void sq_declareFunction(SquirrelContext * sqContext, const char * returnType, const char * functionName, ParamList * parameters);

void sq_declareFunctionType(SquirrelContext * sqContext, const char * returnType, const char * functionName, ParamList * parameters);

void sq_declareEnum(SquirrelContext * sqContext, const char * enumName, NameList * enumValues);

void sq_declareStruct(SquirrelContext * sqContext, const char * structName, AttributeList * attributeList);

void sq_declareNamespace(SquirrelContext * sqContext, const char * namespaceName);

char * sq_ParamValueStringConverter(void * value);

/** Cria tabela de símbolos com valores iniciais (ex.: tipos primitivos)*/
hashtable * sq_createSymbolTable();
/** Destroi tabela de símbolos e todos os valores armazenados.*/
void sq_destroySymbolTable(hashtable * symbolTable);

#endif
