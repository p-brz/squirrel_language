#ifndef SYMBOLTABLE_TYPES_H
#define SYMBOLTABLE_TYPES_H

#include "compiler_types.h"
#include "hashtable.h"

#include "cast_rules.h"

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

typedef struct {
    TypeCategory typeCategory;
}PrimitiveTypeRow;

typedef struct {
    /** O tipo do qual este array foi derivado*/
    char * baseTypename;
}ArrayTypeRow;

//Esta struct será utilizada para tipos de função e funções
typedef struct {
    char * returnTypename;
    arraylist * parameters; //Cada item da lista é um ParamValue
} FunctionRow;

typedef struct {
    char * typeName;
    bool isConst;
    //?escopo
}VariableRow;

//typedef struct NamespaceRow{
//    struct NamespaceRow * parent;
//} NamespaceRow;

typedef struct  {
    //TableRow *type; //TODO: substituir por typename
    char  * typeName;
    bool isConst, isRef;
} ParamValue;

typedef struct  {
    char * type;
    char * name;
} FieldValue;

typedef union {
    StructTypeRow    structValue;
    FieldValue       structFieldValue;
    EnumTypeRow      enumValue;
    FunctionRow      functionValue;
    VariableRow      variableValue;
    PrimitiveTypeRow primitiveValue;
    ArrayTypeRow     arrayValue;
//    NamespaceRow    namespaceValue;
} TableRowValue;

typedef struct TableRow{
    char * name; //mesmo ponteiro da chave (permite acessar nome a partir de referência à TableRow)
    Category category;
    TableRowValue value;
} TableRow;

/* *********************************************************************************************************************************/

bool isType(TableRow * row);
bool isVariable(TableRow * row);


/* ---------------------------------- CONSTRUTORES -----------------------------------*/

//Construtor de table row 
TableRow * sq_TableRow(char * name, Category category, TableRowValue value);

ParamValue * sq_ParamValue(hashtable * symbolTable, const char * typeName, bool isConst, bool isRef);

TableRowValue EmptyRowValue();
TableRowValue sq_PrimitiveTypeValue(TypeCategory typeCategory);
TableRowValue sq_ArrayTypeValue(const char * baseTypename);
TableRowValue sq_FieldRowValue(const FieldValue * field);
TableRowValue VariableRowValue(hashtable * symbolTable, const char * typename, bool isConst);
TableRowValue FunctionRowValue(hashtable * symbolTable, const char * returnType, arraylist * parameters);

/* ---------------------------------- CONVERSORES -----------------------------------*/

/** Converte uma lista de Parameter (ver compiler_helper.h) em uma lista 
    de ParamValue (ver symboltable.h)*/
arraylist * convertToParamValueList(hashtable * symbolTable, ParamList * parameters);

arraylist * convertAttributesToFieldsValues(hashtable * symbolTable,AttributeList * attributeList);

/** Uma função para converter ParamValue em string.
    Ver StringConverter em list_helper.h
*/
char * sq_ParamValueStringConverter(void * value);

#endif //SYMBOLTABLE_TYPES_H