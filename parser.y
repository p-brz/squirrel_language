%{
#include <stdio.h>
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


%start program

%%
program : function_list { printf("function_list\n"); } ;
	
function_list : function { printf( "function\n" ); }
		| function_list	function {};

function : type ID func_params block_body { printf("type ID func_params block_body\n"); };

type : INT { printf( "INT\n" ); };

func_params : LPAREN RPAREN { printf("LPAREN RPAREN\n"); };

block_body : LBRACE block_stmt_list RBRACE { printf("LBRACE block_stmt_list RBRACE\n"); };

block_stmt_list:  | statement_list { printf("  | statement_list\n"); };

statement_list: inline_statement SEMICOLON | statement_list inline_statement SEMICOLON {
printf( "inline_statement SEMICOLON | statement_list inline_statement SEMICOLON\n" ); };

inline_statement : function_call | return_statement { printf("function_call | return_statement\n"); };

function_call : PRINT LPAREN expr_list RPAREN { printf("PRINT LPAREN expr_list RPAREN\n"); };

expr_list : | expr { printf(" | expr\n"); };

return_statement : RETURN expr { printf("RETURN expr\n"); };

expr : NUMBER {printf("NUMBER\n");} | STRING { printf("STRING\n"); };

%%

int main (void) {return yyparse ( );}

int yyerror (char *s) {fprintf (stderr, "%s\n", s);}
