#include "type_checker.h"
#include "stdlib.h"
#include "string_helpers.h"

#include "symboltable.h"

bool checkCategoryEquals(SquirrelContext * sqContext,  TypeCategory current, TypeCategory expected){
    if(current != expected){
        char * errorMsg = concat4("Expecting type category ", sq_typeCategoryCString(expected),
                                   " but found ", sq_typeCategoryCString(current));
        sq_putError(sqContext, errorMsg);
        free(errorMsg);
        return false;
    }
    return true;
}

static 
bool paramEquals(ParamValue * param1, ParamValue * param2){
    return param1 != NULL && param2 != NULL
            && param1->isConst == param2->isConst
            && param1->isRef == param2->isRef
            && strEquals(param1->typeName, param2->typeName);
}

static
bool functionTypesEquals(FunctionRow fRow1, FunctionRow fRow2)
{
    if(strEquals(fRow1.returnTypename, fRow2.returnTypename)){
        arraylist * parameters1 = fRow1.parameters;
        arraylist * parameters2 = fRow2.parameters;
        
        if(parameters1->size == parameters2->size){
            unsigned i;
            for(i=0; i < parameters1->size; ++i){
                ParamValue * param1 = (ParamValue *)arraylist_get(parameters1, i);
                ParamValue * param2 = (ParamValue *)arraylist_get(parameters2, i);
                
                if(! paramEquals(param1, param2)){
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}

bool sq_can_coerce_function_to(SquirrelContext * ctx, 
    const char * functionName, const char * functionTypename)
{
    TableRow * functionRow = sq_findRow(ctx, functionName);
    TableRow * functionTypeRow = sq_findRow(ctx, functionTypename);
    if(functionRow != NULL && functionTypeRow != NULL){
        if(functionRow->category == categ_function
            && functionTypeRow->category == categ_functionType)
        {
            return functionTypesEquals(
                functionRow->value.functionValue, functionTypeRow->value.functionValue);
        }
    }
    return false;
}

type sq_getResultantType(SquirrelContext * ctx, type type1, type type2){
    if(equals_type(type1, type2)){
        return type1;
    }
    else //tipo1 pode ser convertido para tipo 2
    if(can_coerce_to(type1.typename, type1.category, type2.typename, type2.category)){
        return type2;
    }
    else //tipo2 pode ser convertido para tipo 1
    if(can_coerce_to(type2.typename, type2.category, type1.typename, type2.category)){
        return type1;
    }
    else if(type1.category == type_function && type2.category == type_function){
        
    }
    //falha
    return create_Type("", type_invalid, NULL);
}


type sq_getArrayItemType(SquirrelContext * ctx, ExpressionList * arrayItems){
    if(arrayItems != NULL && arrayItems->size > 0){
        Expression * expr1 = (Expression *)arraylist_get(arrayItems, 0);
        type resultantType = create_Type(expr1->type, expr1->typeCategory, NULL);
        
        unsigned i;
        for(i=1; i< arrayItems->size; ++i){
            Expression * exprTmp = (Expression *)arraylist_get(arrayItems, i);
            type otherType = create_Type(exprTmp->type, exprTmp->typeCategory, NULL);
            
            resultantType = sq_getResultantType(ctx, resultantType, otherType);
            if(resultantType.category == type_invalid){
                break;
            }
        }
        
        return resultantType;
    }
    return create_Type("", type_invalid, NULL);
}