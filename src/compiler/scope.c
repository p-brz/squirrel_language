#include "scope.h"

#include "list_helper.h"
#include "string_helpers.h"
#include <stdlib.h>
#include <stdio.h>

void sq_startScope(SquirrelContext * sqContext, const char * scopeName){
    appendList(sqContext->scopeList, cpyString(scopeName));
    char * scopeListStr = sq_fullScopeName(sqContext);
    printf("start  scope %s\n", scopeListStr);
    free(scopeListStr);
    
    //Novo escopo criado
    sqContext->scopeIdCounter++;
}

void sq_finishScope(SquirrelContext * sqContext){
    char * scopeListStr = sq_fullScopeName(sqContext);
    
    char * lastScope = arraylist_pop(sqContext->scopeList);
    if(lastScope != NULL){
        printf("finish scope %s\n", scopeListStr);
        free(lastScope);
    }
    else{
        printf("Error! Finishing unexistent scope");
    }
    
    free(scopeListStr);
}

char * sq_fullScopeName(SquirrelContext * sqContext){
    return sq_makeFullScopeName(sqContext->scopeList);
}
char * sq_makeFullScopeName(arraylist * scopeList){
    return joinList(scopeList, "_", NULL);
}

char * sq_makeScopeId(SquirrelContext * sqContext, const char * prefix){
    char * counterStr = intToString(sqContext->scopeIdCounter);
    char * result = concat(prefix, counterStr);
    free(counterStr);
    
    return result;
}