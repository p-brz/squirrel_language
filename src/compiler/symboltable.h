#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

/** ***************************************************************************************
 symboltable.h - funções para manipulação da tabela de símbolos
  
*******************************************************************************************/

#include "arraylist.h"
#include "hashtable.h"
#include "compiler_types.h"
#include "squirrel_context.h"

//Inclui tipos utilizados pela tabela de símbolos. 
#include "symboltable_types.h"

#include <stdbool.h> //inclui bool em C99


/** ************************ CONSULTA DE SÍMBOLOS *****************************/

/** Busca na tabela de símbolos o tipo de uma variável 'varName'.
    Caso não encontre, retorna NULL.*/
const char * sq_getVarType(SquirrelContext * sqContext, const char * varName);

bool sq_ExistSymbol(SquirrelContext * sqContext, const char * symbol);
/** ************************ DECLARAÇÃO DE SÍMBOLOS *****************************/

/** Insere na tabela de símbolos uma lista de nomes de variáveis (nameDeclList), 
    declaradas com um mesmo tipo (type)*/
void sq_declareVariables(
        SquirrelContext * sqContext, 
        const char * typeName, 
        arraylist * nameDeclList);

/** Similar a sq_declareVariables, mas cria variáveis são marcadas const isConst = true*/
void sq_declareConstants(
        SquirrelContext *   sqContext, 
        const char *        typeName, 
        arraylist *         nameDeclList);

/** Declara função e inicia escopo.
*/
void sq_startFunction(
        SquirrelContext *   sqContext, 
        const char *        returnType, 
        const char *        functionName, 
        ParamList *         parameters);

/** Insere na tabela de símbolos uma função com tipo de retorno 'returnType' e nome 'functionName'.

    @param paremeters - uma lista de ponteiros para Parameter (ver compiler_helper.h)
*/
void sq_declareFunction(
        SquirrelContext * sqContext, 
        const char      * returnType, 
        const char      * functionName, 
        ParamList       * parameters);

void sq_declareFunctionType(
        SquirrelContext *   sqContext, 
        const char *        returnType, 
        const char *        functionName, 
        ParamList *         parameters);

void sq_declareArrayType(
        SquirrelContext * sqContext, 
        const char * originTypeName);

void sq_declareEnum(
        SquirrelContext *   sqContext, 
        const char *        enumName, 
        NameList *          enumValues);

void sq_declareStruct(
        SquirrelContext *   sqContext, 
        const char *        structName, 
        AttributeList *     attributeList);

void sq_declareNamespace(
        SquirrelContext *   sqContext, 
        const char *        namespaceName);

/* ******************************************************************************/

/** Cria tabela de símbolos com valores iniciais (ex.: tipos primitivos)*/
hashtable * sq_createSymbolTable();
/** Destroi tabela de símbolos e todos os valores armazenados.*/
void sq_destroySymbolTable(hashtable * symbolTable);

#endif
