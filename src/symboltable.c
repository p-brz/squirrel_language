#include "symboltable.h"
#include "compiler_helper.h"
#include "string_helpers.h"
#include <stdlib.h>
#include <stdbool.h>

void putVariable(hashtable * symbolTable, const char * typename, NameDeclItem * item, bool isConst);
void declareVariables(hashtable * symbolTable, const char * typename, arraylist * nameDeclList, bool isConst);
void putPrimitive(hashtable * symbolTable, const char * typeName);
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

TableRow * sq_TableRow(char * name, Category category){
    TableRow * row = (TableRow *)malloc(sizeof(TableRow));
    row->category = category;
    row->name = name;
    
    return row;
}

TableRow * sq_findType(hashtable * symbolTable, const char * name){
    TableRow * row = (TableRow *)hashtable_get(symbolTable, (char * )name);
    if(row == NULL || !isType(row)){
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

void declareVariables(hashtable * symbolTable, const char * typename, arraylist * nameDeclList, bool isConst){
    int i;
	void* value;
	//arraylist_iterate gera um for para iterar sobre a lista (ver arraylist.h)
	arraylist_iterate(nameDeclList, i, value) {
	    NameDeclItem * item = (NameDeclItem *) value;
	    putVariable(symbolTable, typename, item, isConst);
	}
}

void putVariable(hashtable * symbolTable, const char * typename, NameDeclItem * item, bool isConst){
    TableRow * row = sq_TableRow(item->name, categ_variable);
    row->value.variableValue.isConst = isConst;
    row->value.variableValue.type = sq_findType(symbolTable, typename);
    //TODO: o nome a ser colocado deve incluir escopo
    hashtable_set(symbolTable, item->name, row);
}
void putPrimitive(hashtable * symbolTable, const char * typeName){
    char * name = cpyString(typeName);
    TableRow * row = sq_TableRow(name, categ_primitiveType);
    hashtable_set(symbolTable, name, row);
}
