#include "compiler_types.h"
#include "string_helpers.h"
#include "list_helper.h"
#include <stdlib.h>

Expression *sq_Expression( sq_type type, const char *exprParam )
{
	Expression *expr = (Expression*)malloc(sizeof(Expression));
	expr->type = type;
	expr->expr = cpyString(exprParam);

	return expr;
}

Parameter * sq_Parameter(const char * typeName, const char * name, arraylist * modifiersList){
	Parameter * param = (Parameter*)malloc(sizeof(Parameter));
	param->type = cpyString(typeName);
	param->name = cpyString(name);
                                                                
	param->isConst = existItem(modifiersList, "const", StrEqualsComparator);
	param->isRef = existItem(modifiersList, "ref", StrEqualsComparator);
	return param;
}

NameDeclItem * sq_NameDeclItem(char * name, Expression * expr){
	NameDeclItem *nameDecl = (NameDeclItem*)malloc(sizeof(NameDeclItem));
    nameDecl->name = name;
    nameDecl->expr = expr;
    return nameDecl;
}

AttributeDecl * sq_AttributeDecl(char * typeName, NameList * namesList){
	AttributeDecl *atributeDecl = (AttributeDecl*)malloc(sizeof(AttributeDecl));
	atributeDecl->type = cpyString(typeName);
	atributeDecl->namesList = namesList;
	
	return atributeDecl;
}

char * sq_NameDeclToString(void * item){
    NameDeclItem * nameDecl = (NameDeclItem *)item;
    if(nameDecl->expr != NULL){
        return concat3(nameDecl->name, " = ", nameDecl->expr->expr);
    }
    return cpyString(nameDecl->name);
}

char * sq_ParameterToString(void * parameter){
    if(parameter == NULL){
        return cpyString("");
    }
    Parameter * param = (Parameter *)parameter;
    
    char * result = cpyString("");
    if(param->isConst == true){
        appendStr(&result, "const ");
    }
    if(param->isRef == true){
        appendStr(&result, "ref ");
    }
    appendStr(&result, param->type);
    appendStr(&result, " ");
    appendStr(&result, param->name);
    
    return result;
}

char *sq_exprToStr( Expression *expr )
{
    switch ( expr->type ) {
        case type_int:
            return concat3("cast_intToStr(", expr->expr, ")");
        break;
    }
    return "";
}

