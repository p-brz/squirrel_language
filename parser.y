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
%token ENUM STRUCT FUNCTION
%token LPAREN RPAREN LBRACE RBRACE
%token PRINT RETURN
%token <sValue> STRING 
%token SEMICOLON COMMA

%token  PLUS MINUS TIMES DIVIDE MOD

%type <sValue> declaration declaration_list
%type <sValue> type_definition enum_definition struct_definition functiontype_definition
%type <sValue> inline_statement function_call io_command return_statement statement_list
%type <sValue> block_body block_stmt_list
%type <sValue> func_params function
%type <sValue> param_decl_list param_decl type_decl
%type <sValue> expr expr_list
%type <sValue> binary_expr term operator
%type <sValue> type
%type <sValue> id_list

%start program

%%
program          : declaration_list                 { printf("%s\n", $1); };

/*OBS.: removida regra de declaration_list vazia, devido a conflito shift-reduce.
    Resolver isto quando modulos forem introduzidos (modulos podem ser vazios?)*/
declaration_list :  declaration                     {   $$ = $1;}
                    | declaration_list declaration  {   const char * values[] = {$1, "\n\n", $2};
                                                        $$ = concat_n(3, values); };

declaration      : type_definition                  {   $$ = $1; }
                      | function                    {   $$ = $1; };

type_definition  : enum_definition                  {   $$ = $1;}
                      | struct_definition           {   $$ = $1;};
                      | functiontype_definition     {   $$ = $1;};
	
/* *********************************** FUNCTIONS ********************************************** */

function        : type ID func_params block_body    {   
                                                        const char * values[] = {$1, " ", $2, " ", $3, $4};
                                                        $$ = concat_n(6, values); };

func_params     : LPAREN param_decl_list RPAREN     {   $$ = concat3("(",$2,")"); };

param_decl_list : /* Vazio*/                                {   $$ = "";}
                      | param_decl                          {   $$ = $1;}
                      | param_decl_list COMMA param_decl    {   $$ = concat3($1, ", ", $3);};

param_decl      : type_decl ID                      {   $$ = concat3($1," ",$2);};

/* ************************************* TYPES *********************************************** */

type_decl       : type                              {   $$ = $1;};

type            : INT                               {   $$ = strdup("int"); };

/* ********************************* TYPE DEFINITION ***************************************** */

enum_definition   : ENUM ID LBRACE id_list RBRACE   {   const char * values[] = {"enum ", $2, "{", $4, "}"};
                                                        $$ = concat_n(5, values);};
                                                        
struct_definition : STRUCT ID 
                    LBRACE var_decl_list RBRACE     {   const char * values[] = {"struct ", $2, "{", 
                                                                                                "TODO: struct_body" , "}"};
                                                        $$ = concat_n(5, values);};
                                                        
functiontype_definition: 
                    FUNCTION type ID func_params    {   const char * values[] = {"function ", $2," ", $3, $4};
                                                        $$ = concat_n(5, values);}

id_list : ID                                        {   $$ = $1;}
            | id_list COMMA  ID                     {   $$ = concat3($1, ",", $3);};
            
//var_decl_list depende de declaração de variáveis
var_decl_list : /*TODO*/                            {};

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
                                                        $$ = concat_n(3, values);};
                    

term            : NUMBER                            { $$ = intToString(yylval.iValue);} 
                    | STRING                        { $$ = strdup(yylval.sValue); }
                    | function_call                 { $$ = $1;};

operator        :   PLUS                            { $$ = strdup("+");}
                    | MINUS                         { $$ = strdup("-");}
                    | TIMES                         { $$ = strdup("*");}
                    | DIVIDE                        { $$ = strdup("/");}
                    | MOD                           { $$ = strdup("%");};
%%

int main (void) {return yyparse ( );}

int yyerror (char *s) {fprintf (stderr, "%s\n", s);}
