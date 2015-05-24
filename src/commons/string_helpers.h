#ifndef STRING_HELPERS_H
#define STRING_HELPERS_H

/** ***************************************************************************************
 Auxiliares para manipulação de strings
  
*******************************************************************************************/

char  * concat(const char * str1, const char * str2);
char  * concat3(const char * str1, const char * str2, const char * str3);
char  * concat_n(int size, const char ** values);
char  * concat4(const char * str1, const char * str2, const char * str3, const char * str4);
char  * cpyString(const char *s);
/** Acrescenta ao final da string 'str' o valor de 'toAppend'. 
    ATENÇÃO: O valor anterior de str é liberado da memória.*/
void appendStr(char ** str, const char * toAppend);

char  * intToString(int value);
char  * longToString(long long value);
char  * realToString(double value);

#endif
