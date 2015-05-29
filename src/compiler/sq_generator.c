#include "sq_generator.h"
#include "string_helpers.h"
#include "symboltable_types.h"
#include "symboltable.h"
#include "hashtable.h"
#include "list_helper.h"
#include "scope.h"

#include <stdlib.h> //NULL

const char * INCLUDES = "#include \"sqlib.h\"\n\n";

const char * MAIN_INCLUDE = "\n#include \"sq_main.incl\"\n";

const char * DEFINE_TYPE_BEGIN = "#define TYPE_";
const char * TYPE_TABLE_DECLARATION = "const type TYPE_TABLE[] = ";

const char * IF_START = "IF_START";
/***************************** FUNÇÕES AUXILIARES *****************************/

char * generateTypeTable(SquirrelContext * ctx);
/*substitui caracteres especiais presentes no nome de um tipo (ex.: '[]') para
  gerar um identificador válido em C.*/
char * gen_TypeIdentifier(const char * typeName);
char * gen_TypeIDDefine(TableRow * tableRow, int typeNumber);
char * gen_TypeTableItem(TableRow * tableRow);
char * gen_SetTypeArrayOf(TableRow * tableRow);
char * getTypeCategoryStr(TableRow * tableRow);
//----------------------------------------------------------------------------//
char * gen_Block(const char * stmts);
char * sq_genScopeDecrementVariables(SquirrelContext * sqContext);
char * sq_genIfBlockStart(
        const char * ifId, const char * conditional_test, const char * block_body);
char * sq_genElseBlock(const char * ifId, const char * elseStmts);
//----------------------------------------------------------------------------//
char * sq_genCString(Expression * expr);
/******************************************************************************/

char * gen_program(SquirrelContext * ctx, const char * declarationsList){
    return concat4(INCLUDES, generateTypeTable(ctx), declarationsList, MAIN_INCLUDE);
}

char * sq_genIfBlock(IfStruct * ifStruct){
    char * ifStart = sq_genIfBlockStart(ifStruct->ifId, ifStruct->conditional_test, ifStruct->block_stmts);
    char * ifEnd = concat3("IF_END(", ifStruct->ifId, ");\n");
    char * result = concat(ifStart, ifEnd);
    free(ifStart);
    free(ifEnd);
    return result;
}

/*
    IF_START(if_id, conditional_test){
        body_stmts
    }
    ELSE_START(if_id){
        else_stmts
    }
    ELSE_END(if_id);
*/
char * sq_genIfElseBlock(IfStruct * ifStruct, const char * elseStmts){
    char * ifStart = sq_genIfBlockStart(ifStruct->ifId, ifStruct->conditional_test, ifStruct->block_stmts);
    char * elseBlock = sq_genElseBlock(ifStruct->ifId, elseStmts);
    return concat(ifStart, elseBlock);
}

/*
    WHILE_START(while_id, conditional_test){
        block_stmts
    WHILE_END(while_id);
        //decrementar variáveis do escopo
    }
*/
char * sq_genWhile(SquirrelContext * sqContext, const char * conditional_test, const char * block_stmts){
    char * whileId = sq_makeScopeId(sqContext, "while_");
    char * whileStart = concat5("WHILE_START(", whileId, ", ",conditional_test, ") {\n");
    char * whileEnd = concat3("WHILE_END(", whileId, ");\n");
    char * decrement_var = sq_genScopeDecrementVariables(sqContext);
    
    char * result = concat5(whileStart, block_stmts, whileEnd, decrement_var, "}\n");
    free(whileId);
    free(whileStart);
    free(whileEnd);
    free(decrement_var);
    return result;
    
}

/*
    DO_START(do_id){
        block_stmts
    DO_END(do_id, conditional_test);
    }
*/
char * sq_genDoWhile(SquirrelContext * sqContext, const char * block_stmts, const char * conditional_test){
    char * doId          = sq_makeScopeId(sqContext, "do_");
    char * doStart       = concat3("DO_START(", doId, ") {\n");
    char * doEnd         = concat5("DO_END(", doId, ", ",conditional_test, ");\n");
    char * decrement_var = sq_genScopeDecrementVariables(sqContext);
    
    char * result = concat5(doStart, block_stmts, doEnd, decrement_var, "}\n");
    free(doStart);
    free(doEnd);
    free(decrement_var);
    return result;
}

/*
    FOR_START(){
    FOR_HEADER(for_id, stmt_init, condition, stmt_incr);
        block_stmts
    
    FOR_END(for_id);
        decrementVar
    }
*/
char * sq_genForBlock(SquirrelContext * sqContext, NameList * forStatements, const char * block_stmts){
    char * forId = sq_makeScopeId(sqContext, "for_");

    if(strlen((const char *)arraylist_get(forStatements, 1)) == 0){
        arraylist_set(forStatements, 1, cpyString("true"));
    }
    char * forParamList = joinList(forStatements, ", ", NULL);
    
    char * forHeader = concat5("FOR_START(){\n FOR_HEADER(", forId, ", ", forParamList, ");\n");
    char * forEnd = concat3("FOR_END(", forId, ");\n");
    char * decrement_var = sq_genScopeDecrementVariables(sqContext);
    
    char * result = concat5(forHeader, block_stmts, forEnd, decrement_var, "}\n");
    free(forId);
    free(forParamList);
    free(forHeader);
    free(forEnd);
    free(decrement_var);
    return result;
}
/******************************************************************************************************/

char * sq_genScopeDecrementVariables(SquirrelContext * sqContext){
    return cpyString("//TODO: Gerar decremento de variáveis (de tipos compostos) do escopo atual \n");
}
/**
    IF_START(if_id, contional_test){
        block_body
    }
*/
char * sq_genIfBlockStart( const char * ifId, const char * conditional_test, const char * block_body){
    const char * ifStartValues[] = {  IF_START, "(", ifId, ", ", conditional_test, ")" };
    char * if_startStr = concat_n(6, ifStartValues);
    char * result = concat4(if_startStr, "{\n", block_body, "}\n");
    free(if_startStr);
    return result;
}
/*
    ELSE_START(if_id){
        else_stmts
    }
    ELSE_END(if_id);
*/
char * sq_genElseBlock(const char * ifId, const char * elseStmts){
    char * elseStart = concat4("ELSE_START(", ifId, ")", gen_Block(elseStmts));
    char * elseEnd = concat3("ELSE_END(", ifId, ");\n");
    char * result = concat(elseStart, elseEnd);
    free(elseStart);
    free(elseEnd);
    return result;
}


char * gen_Block(const char * stmts){
    return concat3("{\n", stmts, "}\n");
}
/******************************************************************************/
char * generateTypeTable(SquirrelContext * ctx){
    hashtable * symbolTable = ctx->symbolTable;
    
    arraylist * typeIDList = createList(NULL);
    arraylist * typeTableList = createList(NULL);
    
    if(symbolTable != NULL){
        int typeCount = 0;
        int i;
        for(i=0; i < symbolTable->capacity; ++i){
            char * key = symbolTable->body[i].key;
            if(key != NULL){
                TableRow * tableRow = (TableRow *)symbolTable->body[i].value;
                if(isType(tableRow)){
                    char * typeID_define = gen_TypeIDDefine(tableRow, typeCount);
                    ++typeCount;
                    appendList(typeIDList, typeID_define);
                    
                    char * typeTableItem = gen_TypeTableItem(tableRow);
                    appendList(typeTableList, typeTableItem);
                    
                }
            }
        } 
    }
    
    char * typeTableItems = joinList(typeTableList, ",\n\t", NULL);
    char * typeTableStr = concat4(TYPE_TABLE_DECLARATION, "{", typeTableItems,"};\n\n");
    free(typeTableItems);
    destroyList(typeTableList);
    
    char * definesStr =  joinList(typeIDList, "\n", NULL);
    destroyList(typeIDList);
    
    char * result = concat3(typeTableStr, definesStr, "\n\n");
    free(typeTableStr);
    free(definesStr);
    
    return result;
}

char * gen_TypeIdentifier(const char * typeName){
    int arrayCount = strCount(typeName, "[]");
    if(arrayCount > 0){
        int baseTypenameLength = findString(typeName, "[]");
        if(baseTypenameLength > 0){
            char * typeIdentifier = getSubstring(typeName, 0, baseTypenameLength);
            
            int i=0;
            for(i=0; i < arrayCount; ++i){
                appendStr(&typeIdentifier, "_ARRAY");
            }
            return typeIdentifier;
        }
    }
    
    return cpyString(typeName);
}
char * gen_TypeIDDefine(TableRow * tableRow, int typeNumber){
    char * numberStr = intToString(typeNumber);
    char * typeIdentifier = gen_TypeIdentifier(tableRow->name);
    char * result = concat4(DEFINE_TYPE_BEGIN, typeIdentifier, " ", numberStr);
    free(numberStr);
    free(typeIdentifier);
    return result;
}

char * gen_TypeTableItem(TableRow * tableRow){
    //{"int", type_integer, NULL}
    const char * typeName = tableRow->name;
    char * typeCategoryStr = getTypeCategoryStr(tableRow);
    
    const char * values[] = { "{ \"", typeName, "\", ", typeCategoryStr, ", NULL }" };
    char * tableItemStr = concat_n(5, values);
    
    return tableItemStr;
}

char * getTypeCategoryStr(TableRow * tableRow){
    switch(tableRow->category){
        case categ_primitiveType:
            return cpyString(sq_typeCategoryCString(tableRow->value.primitiveValue.typeCategory));
        case categ_structType: 
            return cpyString("type_struct");
        case categ_functionType: 
            return cpyString("type_function");
        case categ_enumType:
            return cpyString("type_enum");
    }
    return cpyString("");
}


char * sq_genLenghtExpr(SquirrelContext * ctx ,  Member * member){
    char * type =  sq_getMemberType (ctx, member);
    if (type == NULL){
        return NULL;
    } 
    
    TableRow  * typeRow = sq_findTypeRow(ctx,type);
    if (typeRow->category == categ_arrayType){
        return concat(sq_memberToString(member),".length");
    }
    else if (strEquals(type,"string")){
        
       return concat3("strlen(" , sq_memberToString(member) ,".cstr)" ); 
    }    
    else{
       return NULL; 
    }
    
}

char * printStrConverter(void * value){
    return sq_genCString((Expression *) value);
}

char * sq_genPrint(SquirrelContext * sqContext, ExpressionList *list)
{
    char *print = cpyString("printf(\"");
    int i = 0;
    for ( i = 0; i < list->size; i++ ) {
        appendStr(&print, "%s");
    }
    appendStr(&print, "\\n\"");
    
    char * printExprList = joinList(list, ",", printStrConverter);
    if(list->size > 0){
        char * tmp = concat3(print, ",", printExprList);
        free(print);
        print = tmp;
    }
    appendStr(&print, ")");
    
    
    return print;
}

char * sq_genCString(Expression * expr){
    if(strEquals(expr->type, "string_literal")){
        return cpyString(expr->expr);  
    }
    else if(strEquals(expr->type, "number_literal") || strEquals(expr->type, "real_literal")){
        return concat3("\"", expr->expr, "\"");   
    }
    
    char *result = cpyString("");
    switch (expr->typeCategory) {
        case type_string:
            result = concat3("string_toCString(", expr->expr, ")");
            break;
        case type_boolean:
            result = concat3("boolean_toCString(", expr->expr, ")");
            break;
        case type_integer:
            result = concat3("long_toCString(", expr->expr, ")");
            break;
        case type_real:
            result = concat3("real_toCString(", expr->expr, ")");
            break;
        case type_type:
            result = concat3("typename(", expr->expr, ").cstr");
            break;
        case type_enum:
            result = concat4(expr->type, "_toCString(", expr->expr, ")");
            break;
    }
    
    return result;
}