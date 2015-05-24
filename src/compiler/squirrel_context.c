#include "squirrel_context.h"

#include "symboltable.h"
#include "list_helper.h"
#include <stdlib.h>

SquirrelContext * sq_SquirrelContext(){
    SquirrelContext * context = (SquirrelContext *)malloc(sizeof(SquirrelContext));
    context->symbolTable = sq_createSymbolTable();
    context->scopeList = createList(NULL);
    
    return context;
}
void sq_DestroySquirrelContext(SquirrelContext * context){
    if(context != NULL){
        sq_destroySymbolTable(context->symbolTable);
        arraylist_destroy(context->scopeList);
    }
}
