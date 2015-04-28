%{
  #include "y.tab.h"
%}

%%

[ \t\n]                             ;
[0-9]+                              {yylval.iValue = atoi(yytext); return NUMBER;}
"("                                 {return LPAREN;}
")"                                 {return RPAREN;}
"{"                                 {return LBRACE;}
"}"                                 {return RBRACE;}
"["                                 {return LBRACKET;}
"]"                                 {return RBRACKET;}

"="	                                {return ASSIGN;}

"+"                                 {return(PLUS);}
"-"                                 {return(MINUS);}
"*"                                 {return(TIMES);}
"/"                                 {return(DIVIDE);}
"%"                                 {return(MOD);}

"const"				                {return(CONST);}
"ref"			   	                {return(REF);}

"void"                              {return(VOID);}
"byte"                              {return(BYTE);}
"short"                             {return(SHORT);}
"int"                               {return(INT);}
"long"                              {return(LONG);}
"float"                             {return(FLOAT);}
"double"                            {return(DOUBLE);}
"boolean"                           {return(BOOLEAN);}
"string"                            {return(STRING);}
"object"                            {return(OBJECT);}
"type"                              {return(TYPE);}

"namespace"                         {return NAMESPACE;}
"enum"                              {return ENUM;}
"struct"                            {return STRUCT;}
"function"                          {return FUNCTION;}

"int"                               {return INT;}
"print"                             {return PRINT;}
"return"                            {return RETURN;}
":"                                 {return COLON;}
";"                                 {return SEMICOLON;}
","                                 {return COMMA;}
"."                                 {return DOT;}
\"(\\.|[^"])*\"                     {yylval.sValue = strdup(yytext); return STRING_LITERAL;}
[a-zA-Z][a-zA-Z0-9]*                {yylval.sValue = strdup(yytext); return ID;}

. yyerror("invalid character");

%%

#ifndef yywrap
   int yywrap (void) {return 1;}
#endif
