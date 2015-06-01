#include "type_checker.h"
#include "stdlib.h"
#include "string_helpers.h"

#include "symboltable.h"

static 
bool sq_ExistMember(SquirrelContext * sqContext, Member * member){
    if(member == NULL || member->tableKey == NULL){
        return false;
    }
    TableRow * row = sq_findRow(sqContext, member->tableKey);
    return row != NULL;
}
bool sq_checkExistMember(SquirrelContext * sqContext, Member * member){
    if(!sq_ExistMember(sqContext, member)){
        char * memberStr = sq_memberToString(member);
        char * errMsg = concat3("Member ", memberStr, " not found.");
        sq_putError(sqContext, errMsg);
        free(memberStr);
        free(errMsg);
        
        return false;
    }
    return true;
}
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
    else //tipo2 pode ser convertido para tipo 1
    if(type1.category == type_array && type2.category == type_array){
        if(strEquals(type1.typename, ARRAY_LITERAL_TYPE)){
            return type2;
        }
        else if(strEquals(type2.typename, ARRAY_LITERAL_TYPE)){
            return type1;
        }
    }
    else if((type1.category == type_function || type1.category == type_functionliteral) 
            && (type2.category == type_function || type2.category == type_functionliteral))
    {
        if(sq_can_coerce_function_to(ctx, type1.typename, type2.typename)){
            return type2;
        }
        else if(sq_can_coerce_function_to(ctx, type2.typename, type1.typename)){
            return type1;
        }
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


type sq_getArrayType(SquirrelContext * ctx, ExpressionList * arrayItems){
    if(arrayItems->size == 0){
        //Se não há itens, usa array literal
        return create_Type(ARRAY_LITERAL_TYPE, type_array, NULL);
    }
    type itemType = sq_getArrayItemType(ctx, arrayItems);
    if(itemType.category != type_invalid){
        char * typeName = concat(itemType.typename, "[]");
        
        type arrayType = create_Type(typeName, type_array, NULL);
        return arrayType;
    }
    return create_Type("", type_invalid, NULL);
}

bool sq_can_cast_function_to(SquirrelContext * ctx, 
    const char * functionName, const char * functionNameTarget)
{
    TableRow * functionRow = sq_findRow(ctx, functionName);
    TableRow * functionTarget = sq_findRow(ctx, functionNameTarget);
    if(functionRow != NULL && functionTarget != NULL){
        if(functionRow->category == categ_function
            && functionTarget->category == categ_function)
        {
            return functionTypesEquals(
                functionRow->value.functionValue, functionTarget->value.functionValue);
        }
    }
    return false;
}

bool checkCastRule(SquirrelContext * sqContext
    , const char * typeName, TypeCategory typeCategory, Expression * expr)
{
    type t1 = create_Type(expr->type, expr->typeCategory, NULL);
    type t2 = create_Type(typeName, typeCategory, NULL);
    if(!caststo(t1, t2) && !sq_can_cast_function_to(sqContext,expr->type, typeName)){
        char * errMsg = concat5("Invalid cast from type '", expr->type, "' to type '", typeName, "'");
        sq_putError(sqContext, errMsg);
        free(errMsg);
    }
    return true;
}

type sq_verifyTypeResultingArithmeticOperator(SquirrelContext * sqContext, Expression * expr1,  Expression * expr2)
{
    TypeCategory typeCategoryExpr1 = expr1->typeCategory;
    TypeCategory typeCategoryExpr2 = expr2->typeCategory;
    
    type typeExpr1 = create_Type(expr1->type, typeCategoryExpr1, NULL);
    type typeExpr2 = create_Type(expr2->type, typeCategoryExpr2, NULL);
    
    if ((typeCategoryExpr1 == type_integer || typeCategoryExpr1 == type_real) && (typeCategoryExpr2 == type_integer || typeCategoryExpr2 == type_real)) {
        return sq_getResultantType(sqContext, typeExpr1, typeExpr2);
    }
    else {
        sq_putError(sqContext, "Operando não é compativel com operador!\n");
        return create_Type( "error", type_invalid, NULL);
    }
}

type sq_verifyTypeResultingBitwiseOperator(SquirrelContext * sqContext, Expression * expr1, Expression * expr2)
{
    TypeCategory typeCategoryExpr1 = expr1->typeCategory;
    TypeCategory typeCategoryExpr2 = expr2->typeCategory;
    
    type typeExpr1 = create_Type(expr1->type, typeCategoryExpr1, NULL);
    type typeExpr2 = create_Type(expr2->type, typeCategoryExpr2, NULL);
    
    if ( typeCategoryExpr1 == type_integer && typeCategoryExpr2 == type_integer ) {
        return typeExpr1;
    }
    else {
        sq_putError(sqContext, "Operando não é compativel com operador!\n");
        return create_Type( "error", type_invalid, NULL);
    }
}

type sq_verifyTypeResultingRelationalOperator(SquirrelContext * sqContext, Expression *expr1, Expression *expr2)
{
    TypeCategory typeCategoryExpr1 = expr1->typeCategory;
    TypeCategory typeCategoryExpr2 = expr2->typeCategory;
    
    type typeExpr1 = create_Type(expr1->type, typeCategoryExpr1, NULL);
    type typeExpr2 = create_Type(expr2->type, typeCategoryExpr2, NULL);
    
    if ((typeCategoryExpr1 == type_integer || typeCategoryExpr1 == type_real) && (typeCategoryExpr2 == type_integer || typeCategoryExpr2 == type_real)) {
        return create_Type( "boolean", type_boolean, NULL);
    }
    else {
        sq_putError(sqContext, "Operando não é compativel com operador!\n");
        return create_Type( "error", type_invalid, NULL);
    }
}

type sq_verifyTypeResultingLogicalOperator(SquirrelContext * sqContext, OperatorCategory operatorCategory, Expression *expr1, Expression *expr2)
{
    TypeCategory typeCategoryExpr1 = expr1->typeCategory;
    TypeCategory typeCategoryExpr2 = expr2->typeCategory;
    
    type typeExpr1 = create_Type(expr1->type, typeCategoryExpr1, NULL);
    type typeExpr2 = create_Type(expr2->type, typeCategoryExpr2, NULL);
    
    if (typeCategoryExpr1 == type_boolean  && typeCategoryExpr2 == type_boolean) {
        return create_Type( "boolean", type_boolean, NULL);
    }
    else {
        sq_putError(sqContext, "Operando não é compativel com operador!\n");
        return create_Type( "error", type_invalid, NULL);
    }
}

type sq_getResultingTypeBinary(SquirrelContext * sqContext, OperatorCategory operatorCategory, Expression *expr1, Expression *expr2)
{
    switch ( operatorCategory ) {
        case opcategory_invalid:
            break;
        case opcategory_arithmetic:
            return sq_verifyTypeResultingArithmeticOperator(sqContext, expr1, expr2);
            break;
        case opcategory_bitwise:
            return sq_verifyTypeResultingBitwiseOperator(sqContext, expr1, expr2);
            break;
        case opcategory_relational:
            return sq_verifyTypeResultingRelationalOperator(sqContext, expr1, expr2);
            break;
        case opcategory_logical:
            //return sq_verifyTypeResultingLogicalOperator(sqContext, expr1, expr2);
            break;
    }
    
    return create_Type( "error", type_invalid, NULL);
}

type sq_getBinaryExpressionType(const char * operator, Expression * expr1, Expression * expr2)
{
    
}

type sq_getResultingTypeUnary(SquirrelContext * sqContext, OperatorCategory operatorCategory, Expression * expr){
    switch(operatorCategory){
        case opcategory_invalid:
            break;
        case opcategory_logical:
           if(expr-> typeCategory!= type_boolean){
                sq_putError(sqContext, "operando com tipo invalido para expressao");
                break;
           }
           else{
               return create_Type("boolean",type_boolean,NULL);
           }
           break;
        case opcategory_bitwise:
            if(expr->typeCategory != type_integer){
                sq_putError(sqContext, "operando com tipo invalido para expressao"); 
                break;
           }
           else{
               return create_Type("int",type_integer,NULL);
           }
            break;
        case opcategory_arithmetic:
            if(expr->typeCategory != type_integer && expr->typeCategory !=type_real){
                sq_putError(sqContext, "operando com tipo invalido para expressao");
                break;
           }
           else if(expr->typeCategory == type_integer){
               return create_Type("int",type_integer,NULL);
           }
           else{
               return create_Type("float",type_real,NULL);
           }
            break;
           
    }
    
    return create_Type("",type_invalid, NULL);
}
type sq_getUnaryExpressionType(SquirrelContext * sqContext, const char * operator , Expression * expr){
   OperatorCategory opCategory = sq_getOperatorCategory(operator);
   return sq_getResultingTypeUnary(sqContext,opCategory,expr);
    
}