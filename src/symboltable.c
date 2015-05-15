#include "symboltable.h"
#include "compiler_helper.h"
#include <stdlib.h>

TableRow * sq_TableRow(Category category){
    TableRow * row = (TableRow *)malloc(sizeof(TableRow));
    row->category = category;
    
    return row;
}

void sq_declareVariables(hashtable * symbolTable, sq_type type, arraylist * nameDeclList){
    
    int i;
	void* value;
	//arraylist_iterate gera um for para iterar sobre a lista (ver arraylist.h)
	arraylist_iterate(nameDeclList, i, value) {
	    NameDeclItem * item = (NameDeclItem *) value;
	    TableRow * row = sq_TableRow(categ_variable);
	    //TODO: o nome a ser colocado deve incluir escopo
	    hashtable_set(symbolTable, item->name, row);
	}
}
