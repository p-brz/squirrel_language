#include "symboltable.h"
#include "compiler_helper.h"
#include "string_helpers.h"
#include <stdlib.h>

void putVariable(hashtable * symbolTable, char * typename, NameDeclItem * item, bool isConst);
void declareVariables(hashtable * symbolTable, char * typename, arraylist * nameDeclList, bool isConst);
void putPrimitive(hashtable * symbolTable, char * typeName);

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

TableRow * sq_TableRow(Category category){
    TableRow * row = (TableRow *)malloc(sizeof(TableRow));
    row->category = category;
    
    return row;
}

void sq_declareVariables(hashtable * symbolTable, char * typeName, arraylist * nameDeclList){
    declareVariables(symbolTable, typeName, nameDeclList, false);
}

void sq_declareConstants(hashtable * symbolTable, char * typeName, arraylist * nameDeclList){
    declareVariables(symbolTable, typeName, nameDeclList, true);
}

void declareVariables(hashtable * symbolTable, char * typename, arraylist * nameDeclList, bool isConst){
    int i;
	void* value;
	//arraylist_iterate gera um for para iterar sobre a lista (ver arraylist.h)
	arraylist_iterate(nameDeclList, i, value) {
	    NameDeclItem * item = (NameDeclItem *) value;
	    putVariable(symbolTable, typename, item, isConst);
	}
}

void putVariable(hashtable * symbolTable, char * typename, NameDeclItem * item, bool isConst){
    TableRow * row = sq_TableRow(categ_variable);
    row->value.variableValue.isConst = isConst;
    //TODO: o nome a ser colocado deve incluir escopo
    hashtable_set(symbolTable, item->name, row);
}
void putPrimitive(hashtable * symbolTable, char * typeName){
    TableRow * row = sq_TableRow(categ_primitiveType);
    hashtable_set(symbolTable, cpyString(typeName), row);
}
