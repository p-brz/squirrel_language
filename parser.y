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
%token ENUM STRUCT FUNCTION
%token LPAREN RPAREN LBRACE RBRACE LBRACKET RBRACKET
%token PRINT RETURN
%token <sValue> STRING_LITERAL 
%token SEMICOLON COMMA COLON DOT
%token NAMESPACE
%token PLUS MINUS TIMES DIVIDE MOD

%token ASSIGN
%token CONST REF

%token VOID BYTE SHORT INT LONG FLOAT DOUBLE BOOLEAN STRING OBJECT TYPE



%type <sValue> declaration declaration_list
%type <sValue> namespace type_definition enum_definition struct_definition functiontype_definition
%type <sValue> inline_statement function_call io_command return_statement statement_list
%type <sValue> block_body block_stmt_list struct_body
%type <sValue> func_params function
%type <sValue> param_decl_list param_decl type_decl
%type <sValue> expr expr_list
%type <sValue> binary_expr term value
%type <sValue> operator

%type <sValue> attribute_list variables_decl name_decl_list name_decl
%type <sValue> type_modifier_list type_modifier
%type <sValue> type simple_type array_type
%type <sValue> id_list

%type <sValue> struct_constructor member_init member_init_list
%type <sValue> member

%start program

%%
program          : declaration_list                 { printf("%s\n", $1); };

/*OBS.: removida regra de declaration_list vazia, devido a conflito shift-reduce.
    Resolver isto quando modulos forem introduzidos (modulos podem ser vazios?)*/
declaration_list :  declaration                     {   $$ = $1;}
                    | declaration_list declaration  {   const char * values[] = {$1, "\n\n", $2};
                                                        $$ = concat_n(3, values); };

declaration      : type_definition                  {   $$ = $1; }
                      | function                    {   $$ = $1; }
                      | namespace                   {   $$ = $1; }
                      | variables_decl SEMICOLON    {   $$ = concat($1, ";"); };


type_definition  : enum_definition                  {   $$ = $1;}
                      | struct_definition           {   $$ = $1;}
                      | functiontype_definition     {   $$ = $1;};
                      
namespace        : NAMESPACE ID 
                    LBRACE declaration_list RBRACE  {   const char * values[] = {"namespace ", $2, "{\n", $4, "\n}"};
                                                        $$ = concat_n(5, values); };
                                                        
	
/* *********************************** FUNCTIONS ********************************************** */

function        : type ID func_params block_body    {   const char * values[] = {$1, " ", $2, " ", $3, $4};
                                                        $$ = concat_n(6, values); };

func_params     : LPAREN param_decl_list RPAREN     {   $$ = concat3("(",$2,")"); };

param_decl_list : /* Vazio*/                                {   $$ = "";}
                      | param_decl                          {   $$ = $1;}
                      | param_decl_list COMMA param_decl    {   $$ = concat3($1, ", ", $3);};

param_decl      : type_decl ID                      {   $$ = concat3($1," ",$2);};

/* ************************************* TYPES *********************************************** */

type_decl       : type                              {   $$ = $1;};

type            : simple_type                        {   $$ = $1; }
                    | array_type                     {   $$ = $1; };

simple_type : VOID      { $$ = strdup("void"); }
              | BYTE    { $$ = strdup("byte"); }
              | SHORT   { $$ = strdup("short"); }
              | INT     { $$ = strdup("int");}
              | LONG    { $$ = strdup("long"); } 
              | FLOAT   { $$ = strdup("float"); }
              | DOUBLE  { $$ = strdup("double"); }
              | BOOLEAN { $$ = strdup("boolean"); }
              | STRING  { $$ = strdup("string"); }
              | OBJECT  { $$ = strdup("object"); }
              | TYPE    { $$ = strdup("type"); }
              | ID      { $$ = $1; };

array_type   : simple_type LBRACKET RBRACKET     {   $$ = concat($1, "[]"); };


/* ********************************* TYPE DEFINITION ***************************************** */

enum_definition   : ENUM ID LBRACE id_list RBRACE   {   const char * values[] = {"enum ", $2, "{", $4, "}"};
                                                        $$ = concat_n(5, values);};
                                                        
struct_definition : STRUCT ID 
                    LBRACE struct_body RBRACE       {   const char * values[] = {"struct ", $2, "{\n", $4 , "\n}"};
                                                        $$ = concat_n(5, values);};
struct_body       :  {$$ = "";}
                    | attribute_list { $$ = $1;};
                                                        
functiontype_definition: 
                    FUNCTION type ID func_params    {   const char * values[] = {"function ", $2," ", $3, $4};
                                                        $$ = concat_n(5, values);};

id_list : ID                                        {   $$ = $1;}
            | id_list COMMA  ID                     {   $$ = concat3($1, ",", $3);};
            
attribute_list  :  variables_decl SEMICOLON                      {   $$ = concat($1,";"); }
                    | attribute_list variables_decl SEMICOLON    {   const char *values[] = {$1, "\n", $2, ";"};
                            						                $$ = concat_n(4, values);};

/* ********************************************************************************************* */

struct_constructor  : ID LBRACE member_init_list RBRACE         {   const char * valores[] = {$1, "{", $3, "}"};
                                                                    $$ = concat_n(4, valores);};

member_init_list    :   /*vazio*/                               {   $$ = "";}
                        | member_init                           {   $$ = $1;} 
                        | member_init_list COMMA member_init    {   $$ = concat3($1, ", ", $3);};

member_init         : ID COLON expr                             {   $$ = concat3($1, " : ", $3);};

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
                    | return_statement              {   $$ = $1; }
                    | variables_decl                {   $$ = $1; };

function_call    : member LPAREN expr_list RPAREN   {   const char * values[] = {$1, "(", $3, ")"};
                                                        $$ = concat_n(4, values); }
                    | io_command                    {   $$ = $1;};

io_command       : PRINT LPAREN expr_list RPAREN         {   $$ = concat(concat("print(", $3), ")"); }
                    | "read" LPAREN expr RPAREN     {   $$ = concat(concat("read(", $3), ")"); }
                    | "readchar" LPAREN RPAREN      {   $$ = strdup("readchar()"); }
                    | "readline" LPAREN RPAREN      {   $$ = strdup("readline()"); };


return_statement : RETURN expr                      {   $$ = concat("return ", $2); };

variables_decl   : type name_decl_list { const char *values[] = {$1, " ", $2};
 					      $$ = concat_n(3, values);}
                   | type_modifier_list type name_decl_list { const char *values[] = {$1, " ", $2, " ", $3};
 					      $$ = concat_n(5, values);};

name_decl_list   : name_decl { $$ = $1; }
	           | name_decl_list COMMA name_decl { const char *values[] = {$1, ",", $3};
						      $$ = concat_n(3, values);};


name_decl 	 : ID { $$ = $1; }
		   | ID ASSIGN expr { const char *values[] = {$1, "=", $3}; 
				      $$ = concat_n(3, values);};


type_modifier_list : type_modifier { $$ = $1; }
                       | type_modifier_list type_modifier {const char *values[] = {$1, " ", $2}; $$ = concat_n(3, values);};

type_modifier : CONST { $$ = strdup("const");}
                | REF { $$ = strdup("ref");};

/* ********************************* EXPRESSIONS ********************************************* */
expr_list       : /* Vazio */                       {   $$ = "";}
                    | expr                          {   $$ = $1;}
                    | expr_list COMMA expr          {   $$ = concat(concat($1, ","), $3);};

expr            : binary_expr                       {   $$ = $1;};

binary_expr     : term                              {   $$ = $1;}
                    | binary_expr operator term     {   const char * values[] = {$1, $2, $3};
                                                        $$ = concat_n(3, values);};
                    

term            : function_call                     {   $$ = $1;}
                    | struct_constructor            {   $$ = $1;}
                    | value;
                    
value           : NUMBER                            {   $$ = intToString(yylval.iValue);} 
                    | STRING_LITERAL                {   $$ = strdup(yylval.sValue); };

member          : ID                                {   $$ = $1;}
                    | member DOT ID                 {   $$ = concat3($1,".",$3);};

/* ********************************* OPERATORS ********************************************* */
operator        :   PLUS                            { $$ = strdup("+");}
                    | MINUS                         { $$ = strdup("-");}
                    | TIMES                         { $$ = strdup("*");}
                    | DIVIDE                        { $$ = strdup("/");}
                    | MOD                           { $$ = strdup("%");};
%%

int main (void) {return yyparse ( );}

int yyerror (char *s) {fprintf (stderr, "%s\n", s);}
