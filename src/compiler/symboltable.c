#include "symboltable.h"
#include "string_helpers.h"
#include "list_helper.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> //memset
#include <stdio.h>

//Construtor de table row 
TableRow * sq_TableRow(char * name, Category category, TableRowValue value);

ParamValue * sq_ParamValue(hashtable * symbolTable, const char * typeName, bool isConst, bool isRef);

TableRow * sq_findType(hashtable * symbolTable, const char * name);


void putRow(SquirrelContext * sqContext, const char * name, Category category, const TableRowValue value);
void putRowBase(hashtable * symbolTable, arraylist * scopeList, const char * name, Category category, const TableRowValue value);
void putVariable(SquirrelContext * sqContext, const char * typename, NameDeclItem * item, bool isConst);
void declareVariables(SquirrelContext * sqContext, const char * typename, arraylist * nameDeclList, bool isConst);
void putPrimitive(hashtable * symbolTable, const char * typeName);
void declareParamVariables(SquirrelContext * sqContext, ParamList * parameters);
/** Converte uma lista de Parameter (ver compiler_helper.h) em uma lista de ParamValue (ver symboltable.h)*/
arraylist * convertToParamValueList(hashtable * symbolTable, ParamList * parameters);
arraylist * convertAttributesToFieldsValues(hashtable * symbolTable,AttributeList * attributeList);
TableRowValue EmptyRowValue();
TableRowValue VariableRowValue(hashtable * symbolTable, const char * typename, bool isConst);
TableRowValue FunctionRowValue(hashtable * symbolTable, const char * returnType, arraylist * parameters);

bool isType(TableRow * row){
    if(row != NULL){
        return row->category == categ_primitiveType
                || row->category == categ_structType
                || row->category == categ_functionType
                || row->category == categ_enumType;
    }
    return false;
}

hashtable * sq_createSymbolTable(){
    hashtable * symbolTable = hashtable_create();
    
    putPrimitive(symbolTable, "void");
    putPrimitive(symbolTable, "byte");
    putPrimitive(symbolTable, "short");
    putPrimitive(symbolTable, "int");
    putPrimitive(symbolTable, "long");
    putPrimitive(symbolTable, "float");
    putPrimitive(symbolTable, "double");
    putPrimitive(symbolTable, "boolean");
    putPrimitive(symbolTable, "type");
    putPrimitive(symbolTable, "string");
    putPrimitive(symbolTable, "object");
    
    return symbolTable;
}
void sq_destroySymbolTable(hashtable * symbolTable){
    if(symbolTable == NULL){
        return;
    }

    //TODO: destruir elementos da tabela
    
    hashtable_destroy(symbolTable);
}

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




TableRow * sq_TableRow(char * name, Category category, TableRowValue value){
    TableRow * row = (TableRow *)malloc(sizeof(TableRow));
    row->category = category;
    row->name = name;
    row->value = value;
    
    return row;
}


ParamValue * sq_ParamValue(hashtable * symbolTable, const char * typeName, bool isConst, bool isRef){
    ParamValue * paramValue = (ParamValue *)malloc(sizeof(ParamValue));
    paramValue->type = sq_findType(symbolTable, typeName);
    paramValue->isConst = isConst;
    paramValue->isRef = isRef;
    
    return paramValue;
}
FieldValue * sq_FieldValue(const char * typeName, const char * fieldName){
    FieldValue * fieldValue = (FieldValue *)malloc(sizeof(FieldValue));
    fieldValue->type =cpyString(typeName);
    fieldValue->name = cpyString(fieldName);
    return fieldValue;
}

TableRow * sq_findType(hashtable * symbolTable, const char * name){
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

void putRow(SquirrelContext * sqContext, const char * name, Category category, const TableRowValue value){
    putRowBase(sqContext->symbolTable, sqContext->scopeList, name, category, value);
}
void putRowBase(hashtable * symbolTable, arraylist * scopeList, const char * name, Category category, const TableRowValue value){
    char * scopeName = scopeList != NULL ? (char *)sq_makeFullScopeName(scopeList) : (char *)cpyString("");
    char * key = strlen(scopeName) > 0 ? concat3(scopeName, "_", name)
                                       : concat(scopeName, name);
    TableRow * row = sq_TableRow(key, category, value);
    hashtable_set(symbolTable, key, row);
    
    free(scopeName);
}

void putVariable(SquirrelContext * sqContext, const char * typename, NameDeclItem * item, bool isConst){
    TableRowValue value = VariableRowValue(sqContext->symbolTable, typename, isConst);
    putRow(sqContext, item->name, categ_variable, value);
}
void putPrimitive(hashtable * symbolTable, const char * typeName){
    putRowBase(symbolTable, NULL, typeName, categ_primitiveType, EmptyRowValue());
}

TableRowValue EmptyRowValue(){
    TableRowValue emptyValue;
    memset(&emptyValue, 0, sizeof(TableRowValue));
    return emptyValue;
}

TableRowValue VariableRowValue(hashtable * symbolTable, const char * typename, bool isConst){
    TableRowValue rowValue;
    rowValue.variableValue.isConst = isConst;
    rowValue.variableValue.type = sq_findType(symbolTable, typename);
    return rowValue;
}

TableRowValue FunctionRowValue(hashtable * symbolTable, const char * returnType, arraylist * parameters){
    TableRowValue rowValue = EmptyRowValue();
    rowValue.functionValue.returnType = sq_findType(symbolTable, returnType);
    rowValue.functionValue.parameters = convertToParamValueList(symbolTable, parameters);
    
    //TODO: (?) declarar parâmetros como variáveis pertencentes ao escopo da função
    
    return rowValue;
}

arraylist * convertToParamValueList(hashtable * symbolTable, ParamList * parameters){
    arraylist * list = createList(NULL);
    
    int i;
	void* value;
	arraylist_iterate(parameters, i, value) {
	    Parameter * paremeter = (Parameter *)value;
	    ParamValue * paramValue =  sq_ParamValue(symbolTable, paremeter->type, paremeter->isConst, paremeter->isRef);
	    appendList(list, paramValue);
	}
    
    return list;
}

arraylist * convertAttributesToFieldsValues(hashtable * symbolTable,AttributeList * attributeList){
    arraylist * listFields = createList(NULL);
    
    int i;
	void* value;
	arraylist_iterate(attributeList, i, value) {
	    AttributeDecl * attributeDecl = (AttributeDecl *)value;
	    
	    int j; 
	    void * nameValue;
	    arraylist_iterate(attributeDecl->namesList, j, nameValue) {
	        
	        FieldValue * fieldValue =  sq_FieldValue(attributeDecl->type, cpyString((const char * )nameValue));
	        appendList(listFields, fieldValue);
	    }
	}
    
    return listFields;
}

char * sq_ParamValueStringConverter(void * value){
    if(value == NULL){
        return cpyString("");
    }
    
    ParamValue * paramValue = (ParamValue *)value;
    
    char * result = cpyString("");
    if(paramValue->isConst == true){
        appendStr(&result, "const ");
    }
    if(paramValue->isRef == true){
        appendStr(&result, "ref ");
    }
    if(paramValue->type != NULL){
        appendStr(&result, paramValue->type->name);
    }
    else{
        appendStr(&result, "Unknown type");
    }
    
    return result;
}
