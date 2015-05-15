#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

/** ***************************************************************************************
 symboltable.h - tipos e funções para manipulação da tabela de símbolos
  
*******************************************************************************************/

#include "arraylist.h"
#include "hashtable.h"
#include "sqtypes.h"

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
    TableRow *returnType;
    arraylist * parameters; //Cada item da lista é um ParamValue
} FunctionRow;

typedef struct {
    TableRow * type;
    bool isConst;
    //?escopo
}VariableRow;

typedef struct NamespaceRow{
    struct NamespaceRow * parent;
} NamespaceRow;

typedef struct  {
    TableRow *type;
    bool isConst, isRef;
} ParamValue;

typedef struct  {
    TableRow *type;
    char * name;
} FieldValue;

typedef union {
    StructTypeRow structValue;
    EnumTypeRow enumValue;
    FunctionRow functionValue;
    VariableRow variableValue;
    NamespaceRow namespaceValue;
} TableRowValue;

typedef struct TableRow{
    Category category;
    TableRowValue value;
} TableRow;

//Construtor de table row 
//TODO: incluir valor de TableRow (TableRowValue)
TableRow * sq_TableRow(Category category);

/** Cria tabela de símbolos com valores iniciais (ex.: tipos primitivos)*/
hashtable * sq_createSymbolTable();

/** Insere na tabela de símbolos (symbolTable) uma lista de nomes de variáveis (nameDeclList), 
    declaradas com um mesmo tipo (type)*/
void sq_declareVariables(hashtable * symbolTable, sq_type type, arraylist * nameDeclList);

#endif
