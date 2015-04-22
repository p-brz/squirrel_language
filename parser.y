%{
#include <stdio.h>

#include <string.h> //strlen, strcat, ...
#include <stdlib.h> //malloc

    char  * concat(const char * str1, const char * str2){
        char * strConcat = malloc(strlen(str1) + strlen(str2) + 1);
        
        if(strConcat == NULL){
            return "";
        }
        
        strcpy(strConcat, str1);
        strcat(strConcat, str2);
        return strConcat;
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
    char * intToString(int value){
        char tmp[30];
        sprintf(tmp, "%d", value);
        return strdup(tmp);
    }

%}

%union {
	int    iValue; 	/* integer value */
	char   cValue; 	/* char value */
	char * sValue;  /* string value */
	};

%token <sValue> ID
%token <iValue> NUMBER
%token INT
%token LPAREN RPAREN LBRACE RBRACE
%token PRINT RETURN
%token <sValue> STRING 
%token SEMICOLON

%type <sValue> expr expr_list
%type <sValue> type
%type <sValue> inline_statement function_call return_statement statement_list
%type <sValue> block_body block_stmt_list
%type <sValue> func_params function function_list

%start program

%%
program         : function_list                     { printf("%s\n", $1); } ;
	
/* *********************************** FUNCTIONS ********************************************** */
function_list   : function                          {   $$ = $1; }
		            | function_list	function        {   const char * values[] = {$1, "\n", $2};
                                                        $$ = concat_n(3, values); };

function        : type ID                           {   $<sValue>$ = strdup(yylval.sValue);}
                    func_params block_body          {   const char * idValue = $<sValue>3;
                                                        const char * values[] = {$1, " ", idValue, " ", $4, $5};
                                                        $$ = concat_n(6, values); };

func_params     : LPAREN RPAREN                     { $$ = strdup("()"); };

/* ************************************* TYPES *********************************************** */
type : INT  { $$ = strdup("INT"); };

/* *********************************** STATEMENTS ******************************************** */
block_body       : LBRACE block_stmt_list RBRACE    {   char * begin_block = concat("{\n", $2);
                                                        $$ = concat(begin_block, "}"); };

block_stmt_list  : /* Vazio */                      {  $$ = ""; }
                    | statement_list                {  $$ = $1; };

statement_list   : inline_statement SEMICOLON       { $$ = concat($1, ";");}
                    | statement_list inline_statement SEMICOLON 
                                                    {   char * line1 = concat($1, "\n");
                                                        char * line2 = concat($2, ";\n");
                                                        $$ = concat(line1,line2); };

inline_statement : function_call                    { $$ = $1; }
                    | return_statement              { $$ = $1; };

function_call    : PRINT LPAREN expr_list RPAREN    { $$ = concat(concat("print(", $3), ")"); };

return_statement : RETURN expr                      { $$ = concat("return ", $2); };

/* ********************************* EXPRESSIONS ********************************************* */
expr_list        : /* Vazio */                      { $$ = "";}
                    | expr                          { $$ = $1;};

expr             : NUMBER                           { $$ = intToString(yylval.iValue);} 
                    | STRING                        { $$ = strdup(yylval.sValue); };

%%

int main (void) {return yyparse ( );}

int yyerror (char *s) {fprintf (stderr, "%s\n", s);}
