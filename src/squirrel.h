#ifndef SQUIRREL_H	
#define SQUIRREL_H

char  * concat(const char * str1, const char * str2);
char  * concat3(const char * str1, const char * str2, const char * str3);
char  * concat_n(int size, const char ** values);
char  * concat4(const char * str1, const char * str2, const char * str3, const char * str4);
char  * intToString(int value);


typedef enum {type_int} sq_type;

typedef struct Expression{
    sq_type type;
    char *expr;
}Expression;

Expression *sq_createExpression( sq_type type, const char *expr );

char *sq_exprToStr( Expression *expr );

#endif
