#include "dump_table.h"
#include "compiler_types.h"
#include "symboltable.h"
#include "string_helpers.h"
#include "list_helper.h"

#include <stdio.h>
#include <stdlib.h>


char * varToString(TableRow * row){
    const char * constStr = "";
    if(row->value.variableValue.isConst){
        constStr = "const ";
    }
    TableRow * type = row->value.variableValue.type;
    const char * typename = (type == NULL) ? "<?>" : type->name;
    return concat3(constStr, typename, " variable");
}


char * functionRowToString(TableRow * row){
    const char * returnTypeStr = row->value.functionValue.returnType->name;
    arraylist * paramValues = row->value.functionValue.parameters;
    
    return concat4("function(", joinList(paramValues, ", ", sq_ParamValueStringConverter),"): ", returnTypeStr);
}

char * functionTypeRowToString(TableRow * row){
    const char * returnTypeStr = row->value.functionValue.returnType->name;
    arraylist * paramValues = row->value.functionValue.parameters;
    
    return concat4("function type(", joinList(paramValues, ", ", sq_ParamValueStringConverter),"): ", returnTypeStr);
}

char * enumTypeRowToString(TableRow * row){
    char * enumValuesStr = joinList(row->value.enumValue.identifiers, ", ", NULL);
    
    return concat4("enum ", row->name, ": ", enumValuesStr);
}

char * FieldValueStringConverter(void * value){
    if(value == NULL){
        return cpyString("");
    }
    FieldValue * fieldValue = (FieldValue *)value;
    const char * typeName = fieldValue->type == NULL ? "<unknown type>" : fieldValue->type;
    return concat3(fieldValue->name, " : ", typeName);
}

char * structTypeRowToString(TableRow * row){
    char * attributesListStr = joinList(row->value.structValue.fields, ", ", FieldValueStringConverter);
    
    const char * values[] = {"struct ", row->name, "{ ", attributesListStr, "}"};
    return concat_n(5, values);
}

char * tableRowToString(TableRow * row){
    switch(row->category){
        case categ_primitiveType:
            return cpyString("primitive");
        case categ_structType:
            return structTypeRowToString(row);
        case categ_functionType:
            return functionTypeRowToString(row);
        case categ_enumType:
            return enumTypeRowToString(row);
        case categ_function:
            return functionRowToString(row);
        case categ_variable:
            return varToString(row);
        case categ_namespace:
            return cpyString("namespace");
        default:
            return NULL;
    }
    
}

void dumpRow(char * key, void * value){
    printf("%s \t-->\t ", key);
    if(value == NULL){
        printf("NULL");
    }
    else{
        TableRow * row = (TableRow *)value;
        char * rowStr = tableRowToString(row);
        if(rowStr != NULL){
            printf("%s",rowStr);
            free(rowStr);
        }
    }
    
    printf("\n");
}
void dumpSymbolTable(hashtable * symbolTable){
    printf("\n\n-----------------HASHTABLE-----------------\n");
    if(symbolTable != NULL){
        int i;
        for(i=0; i < symbolTable->capacity; ++i){
            char * key = symbolTable->body[i].key;
            if(key != NULL){
                void * value = symbolTable->body[i].value;
                dumpRow(key, value);
            }
        } 
    
    }
}
