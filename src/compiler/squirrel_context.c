#include "squirrel_context.h"

#include "symboltable.h"
#include "list_helper.h"
#include "string_helpers.h"
#include <stdlib.h>

SquirrelContext * sq_SquirrelContext(){
    SquirrelContext * context = (SquirrelContext *)malloc(sizeof(SquirrelContext));
    context->symbolTable = sq_createSymbolTable();
    context->scopeList = createList(NULL);
    context->scopeIdCounter = 0;
    context->errorList = createList(NULL);
    
    return context;
}
void sq_DestroySquirrelContext(SquirrelContext * context){
    if(context != NULL){
        sq_destroySymbolTable(context->symbolTable);
        arraylist_destroy(context->scopeList);
    }
}

void sq_putError(SquirrelContext * sqContext, const char * message){
    appendList(sqContext->errorList, cpyString(message));
}

int sq_getErrorCount(SquirrelContext * sqContext){
    return arraylist_size(sqContext->errorList);
}

bool sq_hasErrors(SquirrelContext * sqContext){
    return sq_getErrorCount(sqContext) > 0;
}