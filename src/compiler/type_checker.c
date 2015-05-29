#include "type_checker.h"
#include "stdlib.h"
#include "string_helpers.h"


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