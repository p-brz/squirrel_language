#include "compiler_helper.h"
#include "string_helpers.h"
#include <stdlib.h>

Expression *sq_Expression( sq_type type, const char *exprParam )
{
	Expression *expr = (Expression*)malloc(sizeof(Expression));
	expr->type = type;
	expr->expr = cpyString(exprParam);

	return expr;
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

char * sq_NameDeclToString(void * item){
    NameDeclItem * nameDecl = (NameDeclItem *)item;
    if(nameDecl->expr != NULL){
        return concat3(nameDecl->name, " = ", nameDecl->expr->expr);
    }
    return cpyString(nameDecl->name);
}

NameDeclItem * sq_NameDeclItem(char * name, Expression * expr){
	NameDeclItem *nameDecl = (NameDeclItem*)malloc(sizeof(NameDeclItem));
    nameDecl->name = name;
    nameDecl->expr = expr;
    return nameDecl;
}
