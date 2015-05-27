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
int strCount(const char * str, const char * searchString){
    if(str == NULL || searchString == NULL){
        return 0;
    }
    
    size_t substrSize = strlen(searchString);
    
    const char * strEnd = str + strlen(str);
    const char * strIndex = str;
    int count = 0;
    
    do{
        strIndex = strstr(strIndex, searchString);
        if(strIndex != NULL){
            ++count;
            strIndex += substrSize;
        }
    }while(strIndex != NULL && strIndex < strEnd);
    
    return count;
}
int findString(const char * str1, const char * searchString){
    const char * strPos = strstr(str1, searchString);
    return strPos == NULL ? -1 : strPos - str1;
}

char * getSubstring(const char * str, size_t startIndex, size_t length){
    char * substr = calloc(length + 1, sizeof(char));
    strncpy(substr, str + startIndex, length);
    substr[length] = '\0';
    return substr;
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
