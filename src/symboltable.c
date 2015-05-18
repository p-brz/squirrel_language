#include "symboltable.h"
#include "compiler_helper.h"
#include "string_helpers.h"
#include "list_helper.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> //memset
#include <stdio.h>


void putRow(hashtable * symbolTable, const char * name, Category category, const TableRowValue value);
void putVariable(hashtable * symbolTable, const char * typename, NameDeclItem * item, bool isConst);
void declareVariables(hashtable * symbolTable, const char * typename, arraylist * nameDeclList, bool isConst);
void putPrimitive(hashtable * symbolTable, const char * typeName);
/** Converte uma lista de Parameter (ver compiler_helper.h) em uma lista de ParamValue (ver symboltable.h)*/
arraylist * convertToParamValueList(hashtable * symbolTable, ParamList * parameters);

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

TableRow * sq_findType(hashtable * symbolTable, const char * name){
    TableRow * row = (TableRow *)hashtable_get(symbolTable, (char * )name);
    if(row == NULL){
        printf("Could not find type '%s'", name);
        return NULL;
    }
    else if(!isType(row)){
        printf("Found symbol '%s', but is not a type.", name);
        return NULL;
    }
    return row;
}

void sq_declareVariables(hashtable * symbolTable, const char * typeName, arraylist * nameDeclList){
    declareVariables(symbolTable, typeName, nameDeclList, false);
}

void sq_declareConstants(hashtable * symbolTable, const char * typeName, arraylist * nameDeclList){
    declareVariables(symbolTable, typeName, nameDeclList, true);
}

void sq_declareFunction(hashtable * symbolTable, const char * returnType, const char * functionName, arraylist * parameters){
    TableRowValue value = FunctionRowValue(symbolTable, returnType, parameters);
    putRow(symbolTable, functionName, categ_function, value);
}

void declareVariables(hashtable * symbolTable, const char * typename, arraylist * nameDeclList, bool isConst){
    int i;
	void* value;
	//arraylist_iterate gera um for para iterar sobre a lista (ver arraylist.h)
	arraylist_iterate(nameDeclList, i, value) {
	    NameDeclItem * item = (NameDeclItem *) value;
	    putVariable(symbolTable, typename, item, isConst);
	}
}


void putRow(hashtable * symbolTable, const char * name, Category category, const TableRowValue value){
    char * key = cpyString(name);
    TableRow * row = sq_TableRow(key, category, value);
    hashtable_set(symbolTable, key, row);
}

void putVariable(hashtable * symbolTable, const char * typename, NameDeclItem * item, bool isConst){
    TableRowValue value = VariableRowValue(symbolTable, typename, isConst);
    putRow(symbolTable, item->name, categ_variable, value);
}
void putPrimitive(hashtable * symbolTable, const char * typeName){
    putRow(symbolTable, typeName, categ_primitiveType, EmptyRowValue());
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
