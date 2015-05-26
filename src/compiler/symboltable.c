#include "symboltable.h"
#include "string_helpers.h"
#include "list_helper.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> //memset
#include <stdio.h>

//-----------------------------------------Declarações de Funções Locais--------------------------------------------------------//

/** Tenta buscar na tabela por uma linha na tabela com nome 'typeName'.
    Este método deve ser responsável pela resolução de nomes.
    Retorna um ponteiro para a linha na tabela (TableRow) ou NULL caso não exista.*/
TableRow * sq_findRow(SquirrelContext * sqContext, const char * name);

/** Tenta buscar na tabela por um tipo de nome 'typeName'. 
    Retorna um ponteiro para sua linha na tabela (TableRow) ou NULL caso não exista ou não seja um tipo.*/
TableRow * sq_findTypeRow(hashtable * symbolTable, const char * typeName);

void putRow(SquirrelContext * sqContext, const char * name, Category category, const TableRowValue value);
void putRowBase(hashtable * symbolTable, arraylist * scopeList, const char * name, Category category, const TableRowValue value);
void putVariable(SquirrelContext * sqContext, const char * typename, NameDeclItem * item, bool isConst);
void declareVariables(SquirrelContext * sqContext, const char * typename, arraylist * nameDeclList, bool isConst);
void putPrimitive(hashtable * symbolTable, const char * typeName, TypeCategory typeCategory);
void declareParamVariables(SquirrelContext * sqContext, ParamList * parameters);

char * makeTableKey(arraylist * scopeList, const char * name);

/* **********************************************************************************************************************/
hashtable * sq_createSymbolTable(){
    hashtable * symbolTable = hashtable_create();
    
    putPrimitive(symbolTable, "void"            , type_void);
    putPrimitive(symbolTable, "byte"            , type_integer);
    putPrimitive(symbolTable, "short"           , type_integer);
    putPrimitive(symbolTable, "int"             , type_integer);
    putPrimitive(symbolTable, "long"            , type_integer);
    putPrimitive(symbolTable, "float"           , type_real);
    putPrimitive(symbolTable, "double"          , type_real);
    putPrimitive(symbolTable, "boolean"         , type_boolean);
    putPrimitive(symbolTable, "type"            , type_type);
    putPrimitive(symbolTable, "string"          , type_string);
    putPrimitive(symbolTable, "object"          , type_object);
    putPrimitive(symbolTable, "number_literal"  , type_integer);
    putPrimitive(symbolTable, "real_literal"    , type_real);
    
    return symbolTable;
}
void sq_destroySymbolTable(hashtable * symbolTable){
    if(symbolTable == NULL){
        return;
    }

    //TODO: destruir elementos da tabela
    
    hashtable_destroy(symbolTable);
}

/* ************************************************************************************/

const char * sq_getVarType(SquirrelContext * sqContext, const char * varName){
    TableRow * row = sq_findRow(sqContext, varName);
    
    if(row == NULL || ! isVariable(row)){
        return NULL;
    }
    
    return row->value.variableValue.typeName;
}

/*********************************************************************************************************/

void sq_declareVariables(SquirrelContext * sqContext, const char * typeName, arraylist * nameDeclList){
    declareVariables(sqContext, typeName, nameDeclList, false);
}

void sq_declareConstants(SquirrelContext * sqContext, const char * typeName, arraylist * nameDeclList){
    declareVariables(sqContext, typeName, nameDeclList, true);
}


void sq_startFunction(SquirrelContext * sqContext, const char * returnType, const char * functionName, ParamList * parameters){
    sq_declareFunction(sqContext, returnType, functionName, parameters);
    sq_startScope(sqContext, functionName);
    declareParamVariables(sqContext, parameters);
}
void sq_declareFunction(SquirrelContext * sqContext, const char * returnType, const char * functionName, arraylist * parameters){
    TableRowValue value = FunctionRowValue(sqContext->symbolTable, returnType, parameters);
    putRow(sqContext, functionName, categ_function, value);
}

void sq_declareFunctionType(SquirrelContext * sqContext, const char * returnType, const char * functionName, ParamList * parameters){
    TableRowValue value = FunctionRowValue(sqContext->symbolTable, returnType, parameters);
    putRow(sqContext, functionName, categ_functionType, value);
}

void * StringDuplicator(const void * value){
    return cpyString((const char *)value);
}

void sq_declareEnum(SquirrelContext * sqContext, const char * enumName, NameList * enumValues){
    TableRowValue rowValue = EmptyRowValue();
    rowValue.enumValue.identifiers = copyList(enumValues, StringDuplicator);
    putRow(sqContext, enumName, categ_enumType, rowValue);
}


void sq_declareStruct(SquirrelContext * sqContext, const char * structName, AttributeList * attributeList){
    TableRowValue rowValue = EmptyRowValue();
    rowValue.structValue.fields = convertAttributesToFieldsValues(sqContext->symbolTable, attributeList);
    putRow(sqContext, structName, categ_structType, rowValue);
}

void sq_declareNamespace(SquirrelContext * sqContext, const char * namespaceName){
    putRow(sqContext, namespaceName, categ_namespace, EmptyRowValue());
}

/* **************************************** AUXILIARES *********************************************************/

char * makeTableKey(arraylist * scopeList, const char * name){
    char * scopeName = scopeList != NULL ? (char *)sq_makeFullScopeName(scopeList) : (char *)cpyString("");
    char * key = strlen(scopeName) > 0 ? concat3(scopeName, "_", name)
                                       : concat(scopeName, name);
    free(scopeName);
    return key;
}

TableRow * sq_findRow(SquirrelContext * sqContext, const char * name){
    //Por hora não faz resolução de nomes completa
    char * key = makeTableKey(sqContext->scopeList, name);
    TableRow * row = (TableRow *)hashtable_get(sqContext->symbolTable, (char * )key);
    free(key);
    return row;
}

TableRow * sq_findTypeRow(hashtable * symbolTable, const char * name){
    TableRow * row = (TableRow *)hashtable_get(symbolTable, (char * )name);
    if(row == NULL){
        printf("Could not find type '%s'\n", name);
        return NULL;
    }
    else if(!isType(row)){
        printf("Found symbol '%s', but is not a type.\n", name);
        return NULL;
    }

    return row;
}
void putRow(SquirrelContext * sqContext, const char * name, Category category, const TableRowValue value){
    putRowBase(sqContext->symbolTable, sqContext->scopeList, name, category, value);
}
void putRowBase(hashtable * symbolTable, arraylist * scopeList, const char * name, Category category, const TableRowValue value){
    char * key = makeTableKey(scopeList, name);
    TableRow * row = sq_TableRow(key, category, value);
    hashtable_set(symbolTable, key, row);
}


void declareVariables(SquirrelContext * sqContext, const char * typename, arraylist * nameDeclList, bool isConst){
    int i;
	void* value;
	//arraylist_iterate gera um for para iterar sobre a lista (ver arraylist.h)
	arraylist_iterate(nameDeclList, i, value) {
	    NameDeclItem * item = (NameDeclItem *) value;
	    putVariable(sqContext, typename, item, isConst);
	}
}

void declareParamVariables(SquirrelContext * sqContext, ParamList * parameters){
    int i;
	void* value;
	arraylist_iterate(parameters, i, value) {
	    Parameter * parameter = (Parameter *)value;
	    
        //TODO: permitir definição de variável 'ref'
	    TableRowValue value = VariableRowValue(sqContext->symbolTable, parameter->type, parameter->isConst);
        putRow(sqContext, parameter->name, categ_variable, value);
	}
}


void putVariable(SquirrelContext * sqContext, const char * typename, NameDeclItem * item, bool isConst){
    TableRowValue value = VariableRowValue(sqContext->symbolTable, typename, isConst);
    putRow(sqContext, item->name, categ_variable, value);
}
void putPrimitive(hashtable * symbolTable, const char * typeName, TypeCategory typeCategory){
    putRowBase(symbolTable, NULL, typeName, categ_primitiveType, sq_PrimitiveTypeValue(typeCategory));
}



