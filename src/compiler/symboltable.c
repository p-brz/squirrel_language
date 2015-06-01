#include "symboltable.h"
#include "string_helpers.h"
#include "list_helper.h"
#include "scope.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> //memset
#include <stdio.h>

//-----------------------------------------Declarações de Funções Locais--------------------------------------------------------//


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

TypeCategory sq_findTypeCategory(SquirrelContext * sqContext, const char * typeName){
   TableRow * typeRow = sq_findTypeRow(sqContext, typeName);
   if(typeRow != NULL){
       printf("try find category of type %s: ", typeRow->name);
       switch(typeRow->category){
        case categ_primitiveType:
            printf("primitive_type\n");
            return typeRow->value.primitiveValue.typeCategory;
        case categ_arrayType:
            printf("array_type\n");
            return type_array;
        case categ_structType:
            printf("struct_type\n");
            return type_struct;
        case categ_functionType:
        case categ_function://Funções tbm podem ser utilizadas como valores
            printf("function_type\n");
            return type_function;
        case categ_enumType:
            printf("enum_type\n");
            return type_enum;
        default:
            printf("invalid_type\n");
            return type_invalid;
        }
   }
   
   return type_invalid;
}

type sq_getFunctionReturnType(SquirrelContext * sqContext, const char * typeName){
    TableRow * typeRow = sq_findRow(sqContext, typeName);
    if(typeRow != NULL){
        if(typeRow->category == categ_function|| typeRow->category == categ_functionType){
            const char * returnTypename = typeRow->value.functionValue.returnTypename;
            if(returnTypename != NULL){
                TypeCategory typeCategory = sq_findTypeCategory(sqContext, returnTypename);
                return create_Type(returnTypename, typeCategory, NULL);
            }
        }
    }
    return create_Type("", type_invalid, NULL);
}

type sq_findArrayItemType(SquirrelContext * sqContext, const char * typeName){
    TableRow * typeRow = sq_findTypeRow(sqContext, typeName);
    if(typeRow != NULL && typeRow->category == categ_arrayType){
        const char * itemType = typeRow->value.arrayValue.baseTypename;
        TypeCategory typeCategory = sq_findTypeCategory(sqContext, itemType);
        
        printf("found type category: %s\n", sq_typeCategoryCString(typeCategory));
        
        return create_Type(itemType, typeCategory, NULL);
    }
    return create_Type("", type_invalid ,NULL);
}
const char * sq_findFullName(SquirrelContext * sqContext, const char * symbolName){
    if(symbolName == NULL){
        return NULL;
    }
    TableRow * row = sq_findRow(sqContext, symbolName);
    return row == NULL ? NULL : row->name;
}
bool sq_ExistSymbol(SquirrelContext * sqContext, const char * symbol){
    return sq_findRow(sqContext, symbol) != NULL;
}

bool sq_ExistSymbolOnScope(SquirrelContext * sqContext, const char * symbol){
    char * key = makeTableKey(sqContext->scopeList,symbol);
    TableRow * row = (TableRow *)hashtable_get(sqContext->symbolTable, (char * )key);
    free(key);
    return row != NULL;
}
#include <stdio.h>

Category sq_findSymbolCategory(SquirrelContext * sqContext, const char * symbol){
    if(symbol == NULL){
        return categ_unknown;
    }
    TableRow * row = sq_findRow(sqContext, symbol);
    return row == NULL ? categ_unknown : row->category;
}

char * sq_makeMemberTableKey(SquirrelContext * sqContext, const char * memberName, const Member * parent){
    if(parent == NULL){
        return cpyString(memberName);
    }
    
    if(parent->category == categ_namespace || parent->category == categ_enumType){
        return concat3(parent->tableKey, "_", memberName);
    }
    else if(parent->category == categ_variable || parent->category == categ_structField){//Pai é uma variável ou campo de uma variável
        TableRow * parentRow = sq_findRow(sqContext, parent->tableKey);
        if(parentRow != NULL){
            TableRow * typeRow = NULL;
            if(parentRow->category == categ_variable){
                typeRow = sq_findTypeRow(sqContext, parentRow->value.variableValue.typeName);
            }
            else if(parentRow->category == categ_structField){
                typeRow = sq_findTypeRow(sqContext, parentRow->value.structFieldValue.type);
            }
            
            if(typeRow != NULL && typeRow->category == categ_structType){//apenas variáveis struct podem ter membros aninhados
                return concat3(typeRow->name, ".", memberName);
            }
        }
    }
    
    return NULL;
}

/**
    Um membro podde ser:
      - um namespace
      - tipo definido por usuário (struct, enum, function)
      - uma variável
      - um campo de uma variável (ou de outro campo)
      - uma função
*/
Category sq_findMemberCategory(SquirrelContext * sqContext, const char * memberName, const Member * parent){
    char * memberSymbolKey = sq_makeMemberTableKey(sqContext, memberName, parent);
    return sq_findSymbolCategory(sqContext, memberName);
}
char * sq_getMemberType (SquirrelContext * ctx ,  const Member * member){
    TableRow * row = sq_findRow(ctx,member->tableKey);
    if(row== NULL){
        return NULL;
    }
    if(row -> category == categ_variable){
        return row -> value.variableValue.typeName ;
    }
    else if(row->category == categ_structField){
        return row->value.structFieldValue.type ;
    }
    else if(row->category == categ_enumFieldValue){//Member pode ser um valor de enum
        return row->value.enumFieldValue.type;
    }
    else{
        return NULL;
    }
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
    printf("sq_declareFunctionType: %s\n", functionName);
    if(sq_ExistSymbolOnScope(sqContext, functionName)){
        char * errMsg = concat3("symbol '", functionName, " already declared on current scope.");
        sq_putError(sqContext, errMsg);
        free(errMsg);
        return;
    }
    TableRowValue value = FunctionRowValue(sqContext->symbolTable, returnType, parameters);
    putRow(sqContext, functionName, categ_functionType, value);
}


void sq_declareArrayType(SquirrelContext * sqContext, const char * originTypeName){
    TableRow * originTypeRow = sq_findTypeRow(sqContext, originTypeName);
    if(originTypeRow == NULL){
        printf("error: Trying to make array type with unknown origin type : '%s'\n", originTypeName);
        return;
    }
    const char * originTypeKey = originTypeRow->name; 
    TableRowValue value = sq_ArrayTypeValue(originTypeKey);
    char * arrayType = concat(originTypeKey, "[]");

    putRowBase(sqContext->symbolTable, NULL, arrayType, categ_arrayType, value);
    free(arrayType);
}

void * StringDuplicator(const void * value){
    return cpyString((const char *)value);
}

void sq_declareEnum(SquirrelContext * sqContext, const char * enumName, NameList * enumValues){
    TableRowValue rowValue = EmptyRowValue();
    rowValue.enumValue.identifiers = copyList(enumValues, StringDuplicator);
    putRow(sqContext, enumName, categ_enumType, rowValue);
    
    int i;
    void * value;
    arraylist_iterate(enumValues, i, value){
        TableRowValue enumFieldValue = sq_EnumFieldRowValue(enumName, (const char *)value);
        char * key = concat3(enumName, "_", (const char *)value);
        putRow(sqContext, key, categ_enumFieldValue, enumFieldValue);
        free(key);
    }
}


void sq_declareStruct(SquirrelContext * sqContext, const char * structName, AttributeList * attributeList){
    TableRowValue rowValue = EmptyRowValue();
    rowValue.structValue.fields = convertAttributesToFieldsValues(sqContext->symbolTable, attributeList);
    putRow(sqContext, structName, categ_structType, rowValue);
    
    int i;
    void * value;
    arraylist_iterate(rowValue.structValue.fields, i, value){
        FieldValue * field = (FieldValue *)value;
        char * fieldName = concat3(structName, "." ,field->name);
        putRow(sqContext, fieldName, categ_structField, sq_FieldRowValue(field));
        free(fieldName);
    }
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
    arraylist * scopeList = sqContext->scopeList->size > 0 ? 
                                    arraylist_copy(sqContext->scopeList) : createList(NULL);
    int i;
    int listSize = arraylist_size(scopeList);
    for(i=listSize; i >=0; --i){
        char * key = makeTableKey(scopeList, name);
        TableRow * row = (TableRow *)hashtable_get(sqContext->symbolTable, (char * )key);
        free(key);
        
        if(row != NULL){
            arraylist_destroy(scopeList);
            return row;
        }
        if(i > 0){
            arraylist_pop(scopeList);
        }
    }
    arraylist_destroy(scopeList);
    return NULL;
}

TableRow * sq_findTypeRow(SquirrelContext * sqContext, const char * name){
    //FIXME: fazer busca com base nos escopos
    TableRow * row = (TableRow *)hashtable_get(sqContext->symbolTable, (char * )name);
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
    
    printf("put row for name '%s' with key '%s'\n", name, key);
    
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



