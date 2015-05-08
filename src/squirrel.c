#include "squirrel.h" 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>



    char  * concat(const char * str1, const char * str2){
        char * strConcat = malloc(strlen(str1) + strlen(str2) + 1);
        
        if(strConcat == NULL){
            return "";
        }
        
        strcpy(strConcat, str1);
        strcat(strConcat, str2);
        return strConcat;
    }
    char  * concat3(const char * str1, const char * str2, const char * str3){
        char * strCat1 = concat(str1, str2);
        char * strCat2 = concat(strCat1, str3);
        free(strCat1);
        return strCat2;
    }
    char  * concat_n(int size, const char ** values){
        if(size <= 0){
            return "";
        }
        
        char * strConcat = strdup(values[0]);
        int i;
        for(i=1; i < size; ++i){
            char * new_concat = concat(strConcat, values[i]);
            free(strConcat);
            strConcat = new_concat;
        }
        
        return strConcat;
    }
    char  * concat4(const char * str1, const char * str2, const char * str3, const char * str4){
        const char * values[] = {str1, str2, str3, str4};
        return concat_n(4, values);
    }
    char * intToString(int value){
        char tmp[30];
        sprintf(tmp, "%d", value);
        return strdup(tmp);
    }

Expression *sq_createExpression( sq_type type, const char *exprParam )
{
	Expression *expr = (Expression*)malloc(sizeof(Expression));
	expr->type = type;
	expr->expr = strdup(exprParam);
	
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

