#include "compiler_types.h"
#include "string_helpers.h"
#include "list_helper.h"
#include <stdlib.h>

Expression *sq_Expression( const char *type, const char *exprParam )
{
	Expression *expr = (Expression*)malloc(sizeof(Expression));
	expr->type = cpyString(type);
	expr->expr = cpyString(exprParam);

	return expr;
}

Member * sq_Member(const char * name, Category category, Member * parent){
    Member * member = (Member *)malloc(sizeof(Member));
    member->name = cpyString(name);
    member->category = category;
    member->parent = parent;
}
void sq_destroyMember(Member * member){
    if(member != NULL){
        sq_destroyMember(member->parent);
        free(member->name);
        free(member);
    }
}
char * sq_memberToString(Member * member){
    if(member == NULL){
        return NULL;
    }
    if(member->parent == NULL){
        return cpyString(member->name);
    }
    else{
        char * parentString = sq_memberToString(member->parent);
        char * result = concat3(parentString, ".", member->name);
        free(parentString);
        return result;
    }
}
Parameter * sq_Parameter(const char * typeName, const char * name, arraylist * modifiersList){
	Parameter * param = (Parameter*)malloc(sizeof(Parameter));
	param->type = cpyString(typeName);
	param->name = cpyString(name);
                                                                
	param->isConst = existItem(modifiersList, "const", StrEqualsComparator);
	param->isRef = existItem(modifiersList, "ref", StrEqualsComparator);
	return param;
}

NameDeclItem * sq_NameDeclItem(const char * name, Expression * expr){
	NameDeclItem *nameDecl = (NameDeclItem*)malloc(sizeof(NameDeclItem));
    nameDecl->name = cpyString(name);
    nameDecl->expr = expr;
    return nameDecl;
}

AttributeDecl * sq_AttributeDecl(const char * typeName, NameList * namesList){
	AttributeDecl *atributeDecl = (AttributeDecl*)malloc(sizeof(AttributeDecl));
	atributeDecl->type = cpyString(typeName);
	atributeDecl->namesList = namesList;
	
	return atributeDecl;
}

IfStruct * sq_IfStruct(char * ifId, char * conditional_test, char * block_stmts){
    IfStruct * ifStruct = (IfStruct *)malloc(sizeof(IfStruct));
    ifStruct->ifId = ifId;
    ifStruct->conditional_test = conditional_test;
    ifStruct->block_stmts = block_stmts;
    return ifStruct;
}

NameList * sq_ForHeader(char * initStmt, char * testConditional, char * incrStmt){
    NameList * list = createList(initStmt);
    appendList(list, testConditional);
    appendList(list, incrStmt);
    return list;
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
    /*switch ( expr->type ) {
        case "int":
            return concat3("cast_intToStr(", expr->expr, ")");
        break;
    }*/
    return cpyString(expr->expr);
}

char * AttributeDeclStringConverter(void * value){
    if(value == NULL){
        return cpyString("");
    }
    AttributeDecl * attributeDecl = (AttributeDecl *)value;
    char * nameDeclListStr = joinList(attributeDecl->namesList, ", ", NULL);
    char * result = concat3(attributeDecl->type, " ", nameDeclListStr);
    free(nameDeclListStr);
    return result;
}

char * attributeListToString(AttributeList * attributeList){
    char * listStr =  joinList(attributeList, ";\n", AttributeDeclStringConverter);
    char * result = concat(listStr, ";");//último ';' não é adicionado por joinList
    free(listStr);
    return result;
}

