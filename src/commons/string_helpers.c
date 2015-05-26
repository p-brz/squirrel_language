#include "string_helpers.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//Baseado em: http://stackoverflow.com/questions/1701055/what-is-the-maximum-length-in-chars-needed-to-represent-any-double-value
#define MAX_REAL_DIGITS 1080

char * cpyString(const char *s) {
    char *d = malloc (strlen (s) + 1);   // Space for length plus nul
    if (d == NULL) return NULL;          // No memory
    strcpy (d,s);                        // Copy the characters
    return d;                            // Return the new string
}

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

    char * strConcat = cpyString(values[0]);
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

void appendStr(char ** str, const char * toAppend){
    char * result = concat(*str, toAppend);
    free(*str);
    *str = result;
}


int strEquals(const char * str1, const char * str2){
    return strcmp(str1, str2) == 0;
}

char  * intToString(int value){
    return longToString(value);
}
char  * longToString(long long value){
    char tmp[30];
    sprintf(tmp, "%lld", value);
    return cpyString(tmp);
}

char * realToString(double value){
    char tmp[MAX_REAL_DIGITS];
    sprintf(tmp, "%f", value);
    return cpyString(tmp);
}
