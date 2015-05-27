#ifndef STRING_HELPERS_H
#define STRING_HELPERS_H

/** ***************************************************************************************
 Auxiliares para manipulação de strings
  
*******************************************************************************************/

#include <string.h> //size_t

char  * concat(const char * str1, const char * str2);
char  * concat3(const char * str1, const char * str2, const char * str3);
char  * concat_n(int size, const char ** values);
char  * concat4(const char * str1, const char * str2, const char * str3, const char * str4);
char  * cpyString(const char *s);
/** Acrescenta ao final da string 'str' o valor de 'toAppend'. 
    ATENÇÃO: O valor anterior de str é liberado da memória.*/
void appendStr(char ** str, const char * toAppend);

int strEquals(const char * str1, const char * str2);
/** Retorna o número de 'searchString' encontradas em 'str1'*/
int strCount(const char * str1, const char * searchString);
/** Retorna o índice da primeira ocorrência de 'searchString' encontradaa em 
    'str1' ou um valor negativo se 'searchString' não for encontrada.*/
int findString(const char * str1, const char * searchString);
char * getSubstring(const char * str, size_t startIndex, size_t length);

char  * intToString(int value);
char  * longToString(long long value);
char  * realToString(double value);


#endif
