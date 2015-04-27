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
%token SEMICOLON COMMA

%token  PLUS MINUS TIMES DIVIDE MOD

%type <sValue> inline_statement function_call io_command return_statement statement_list
%type <sValue> block_body block_stmt_list
%type <sValue> func_params function function_list
%type <sValue> expr expr_list
%type <sValue> binary_expr term operator
%type <sValue> type
%type <sValue> for if while do_while try_catch switch switch_body when_list when_block default_block conditional_test

%start program

%%
program         : function_list                     { printf("%s\n", $1); } ;
	
/* *********************************** FUNCTIONS ********************************************** */
function_list   : function                          {   $$ = $1; }
		            | function_list	function        {   const char * values[] = {$1, "\n", $2};
                                                        $$ = concat_n(3, values); };

function        : type ID func_params block_body    {   
                                                        const char * values[] = {$1, " ", $2, " ", $3, $4};
                                                        $$ = concat_n(6, values); };

func_params     : LPAREN RPAREN                     { $$ = strdup("()"); };

/* ************************************* TYPES *********************************************** */
type : INT  { $$ = strdup("INT"); };

/* *********************************** STATEMENTS ******************************************** */
block_body       : LBRACE block_stmt_list RBRACE    {   char * begin_block = concat("{\n", $2);
                                                        $$ = concat(begin_block, "}\n"); };

block_stmt_list  : /* Vazio */                      {  $$ = ""; }
                    | statement_list                {  $$ = $1; };

statement_list   : inline_statement SEMICOLON       { $$ = concat($1, ";\n");}
                    | statement_list inline_statement SEMICOLON 
                                                    {   char * line1 = $1;
                                                        char * line2 = concat($2, ";\n");
                                                        $$ = concat(line1,line2); };

inline_statement : function_call                    {   $$ = $1; }
                    | return_statement              {   $$ = $1; };

function_call    : ID LPAREN expr_list RPAREN       {   const char * values[] = {$1, "(", $3, ")"};
                                                        $$ = concat_n(4, values); }
                    | io_command                    {   $$ = $1;};

io_command       : PRINT LPAREN expr_list RPAREN         {   $$ = concat(concat("print(", $3), ")"); }
                    | "read" LPAREN expr RPAREN     {   $$ = concat(concat("read(", $3), ")"); }
                    | "readchar" LPAREN RPAREN      {   $$ = strdup("readchar()"); }
                    | "readline" LPAREN RPAREN      {   $$ = strdup("readline()"); };


return_statement : RETURN expr                      {   $$ = concat("return ", $2); };

/* ********************************* EXPRESSIONS ********************************************* */
expr_list       : /* Vazio */                       { $$ = "";}
                    | expr                          { $$ = $1;}
                    | expr_list COMMA expr          { $$ = concat(concat($1, ","), $3);};

expr            : binary_expr                       { $$ = $1;};

binary_expr     : term                              { $$ = $1;}
                    | binary_expr operator term     {   const char * values[] = {$1, $2, $3};
                                                        $$ = concat_n(3, values);}
                    

term            : NUMBER                            { $$ = intToString(yylval.iValue);} 
                    | STRING                        { $$ = strdup(yylval.sValue); }
                    | function_call                 { $$ = $1;};

operator        :   PLUS                            { $$ = strdup("+");}
                    | MINUS                         { $$ = strdup("-");}
                    | TIMES                         { $$ = strdup("*");}
                    | DIVIDE                        { $$ = strdup("/");}
                    | MOD                           { $$ = strdup("%");};
                    
                    /* ********************** BLOCK STATEMENTS *********************** */

block_statement :         for       		{$$ = $1;}
			| if       		{$$ = $1;}
			| while     		{$$ = $1;}
			| do_while  		{$$ = $1;} 
			| try_catch 		{$$ = $1;}
			| switch    		{$$ = $1;};


for             :   "for" "(" std_statement ";" binary_expr ";" std_statement ")" block_body {const char * values[] = {"for(",$3,";", $5, ";",$7 ,")",$9 };
                                                        $$ = concat_n(8, values);};

if 		: if_block				{$$ = $1}
    		        | if_block "else" block_body	{const char * values[] = {$1, "else", $3};
                                                        $$ = concat_n(3, values);}
   			| if_block "else" if		{const char * values[] = {$1, "else", $3};
                                                        $$ = concat_n(3, values);};

if_block 	: "if" conditional_test block_body 	{const char * values[] = {"if", $2, $3};
                                                        $$ = concat_n(3, values);};

while 		: "while" conditional_test block_body  {const char * values[] = {"while", $2, $3};
                                                        $$ = concat_n(3, values);};

do_while        : "do" block_body "while" conditional_test ";"   {const char * values[] = {"do",$2,"while", $4, ";"};
                                                        $$ = concat_n(5, values);};

try_catch 	: "try" block_body "catch" block_body   {const char * values[] = {"try",$2,"catch", $4};
                                                        $$ = concat_n(4, values);};

switch          : "switch" "(" expr ")" "{" switch_body "}"  {const char * values[] = {"switch (", $3, ") {", $6};
                                                        $$ = concat_n(4, values);};

switch_body 	: when_list				{$$ = $1;}
		| when_list default_block		{const char * values[] = {$1, $2};
                                                        $$ = concat_n(2, values);};

when_list       : when_block				{$$ = $1;}
	        | when_list when_block			{const char * values[] = {$1, $2};
                                                        $$ = concat_n(2, values);};

when_block 	: "when" conditional_test block_body    {const char * values[] = {"when", $2, $3};
                                                        $$ = concat_n(3, values);};

default_block   : "default" block_body                  {const char * values[] = {"default", $2};
                                                        $$ = concat_n(2, values);};

conditional_test : "(" binary_expr ")"			{const char * values[] = {"(", $2, ")"};
                                                        $$ = concat_n(3, values);};



%%

int main (void) {return yyparse ( );}

int yyerror (char *s) {fprintf (stderr, "%s\n", s);}
