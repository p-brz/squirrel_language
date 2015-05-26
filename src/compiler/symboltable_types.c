#include "symboltable_types.h"

#include "string_helpers.h"
#include "list_helper.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> //memset
#include <stdio.h>

bool isVariable(TableRow * row){
    if(row  != NULL){
        return row->category == categ_variable;
    }
    return false;
}
bool isType(TableRow * row){
    if(row != NULL){
        return row->category == categ_primitiveType
                || row->category == categ_structType
                || row->category == categ_functionType
                || row->category == categ_enumType;
    }
    return false;
}

/* *************************** CONSTRUTORES ***************************************/

TableRowValue EmptyRowValue(){
    TableRowValue emptyValue;
    memset(&emptyValue, 0, sizeof(TableRowValue));
    return emptyValue;
}

TableRowValue sq_PrimitiveTypeValue(TypeCategory typeCategory){
    TableRowValue rowValue = EmptyRowValue();
    rowValue.primitiveValue.typeCategory = typeCategory;
    return rowValue;
}
TableRowValue sq_ArrayTypeValue(const char * baseTypename){
    TableRowValue rowValue = EmptyRowValue();
    rowValue.arrayValue.baseTypename = cpyString(baseTypename);
    return rowValue;
}
TableRowValue VariableRowValue(hashtable * symbolTable, const char * typename, bool isConst){
    TableRowValue rowValue;
    rowValue.variableValue.isConst = isConst;
    rowValue.variableValue.typeName = cpyString(typename);
    return rowValue;
}

TableRowValue FunctionRowValue(hashtable * symbolTable, const char * returnType, arraylist * parameters){
    TableRowValue rowValue = EmptyRowValue();
    rowValue.functionValue.returnTypename = cpyString(returnType);
    rowValue.functionValue.parameters = convertToParamValueList(symbolTable, parameters);
    
    //TODO: (?) declarar parâmetros como variáveis pertencentes ao escopo da função
    
    return rowValue;
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
    //paramValue->type = sq_findType(symbolTable, typeName);
    paramValue->typeName = cpyString(typeName);
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

/* *************************** CONVERSORES ***************************************/

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
    if(paramValue->typeName != NULL){
        appendStr(&result, paramValue->typeName);
    }
    else{
        appendStr(&result, "Unknown type");
    }
    
    return result;
}