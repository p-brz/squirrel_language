#include "scope.h"

#include "list_helper.h"
#include "string_helpers.h"
#include <stdlib.h>
#include <stdio.h>

void sq_startScope(SquirrelContext * sqContext, const char * scopeName){
    appendList(sqContext->scopeList, cpyString(scopeName));
    char * scopeListStr = joinList(sqContext->scopeList, ".", NULL);
    printf("start  scope %s\n", scopeListStr);
    free(scopeListStr);
}

void sq_finishScope(SquirrelContext * sqContext){
    char * scopeListStr = joinList(sqContext->scopeList, ".", NULL);
    
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
