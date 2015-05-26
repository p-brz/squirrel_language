%{
  #include "squirrel.h"
  #include "y.tab.h"
%}

%%

\[(\ |\t)*\]                        {return ARRAY_SYMBOL;}
[ \t\n]                             ;
0\.[0-9]+                           {yylval.sValue = yytext; return REAL_LITERAL;}
[1-9][0-9]*"."[0-9]+                {yylval.sValue = yytext; return REAL_LITERAL;}
0                                   {yylval.sValue = yytext; return NUMBER_LITERAL;}
[1-9][0-9]*                         {yylval.sValue = yytext; return NUMBER_LITERAL;}

"true"                              {yylval.sValue = yytext; return BOOLEAN_LITERAL;}
"false"                             {yylval.sValue = yytext; return BOOLEAN_LITERAL;}

"("                                 {return LPAREN;}
")"                                 {return RPAREN;}
"{"                                 {return LBRACE;}
"}"                                 {return RBRACE;}
"["                                 {return LBRACKET;}
"]"                                 {return RBRACKET;}

"="                                 {return ASSIGN;}
"+="                                {return ASSIGNPLUS;}
"-="                                {return ASSIGNMINUS;}
"/="                                {return ASSIGNDIVIDE;}
"%="                                {return ASSIGNMOD;}
"|="                                {return ASSIGNBITOR;}
"&="                                {return ASSIGNBITAND;}

"!"                                 {return EXCLAMATION;}
"not"                               {return NOT;}
"~"                                 {return TIO;}

"+"                                 {return(PLUS);}
"-"                                 {return(MINUS);}
"*"                                 {return(TIMES);}
"/"                                 {return(DIVIDE);}
"%"                                 {return(MOD);}
"||"                                {return(OU);}
"or"                                {return(OR);}
"|"                                 {return(BITOR);}
"&"                                 {return(BITAND);}
"&&"                                {return(EE);}
"and"                               {return(AND);}
"++"                                {return(PLUSPLUS);}
"--"                                {return(MINUSMINUS);}
"<<"                                {return(SHIFTL);}
">>"                                {return(SHIFTR);}
"=="                                {return(EQUAL);}
"!="                                {return(DIFERENT);}
"<"                                 {return(MINOR);}
">"                                 {return(BIGGER);}
"<="                                {return(MINOREQUAL);}
">="                                {return(BIGGEREQUAL);}

"new"                               {return NEW;}

"typeof"                            {return(TYPEOF);}
"caststo"                           {return(CASTSTO);}
"typename"                          {return(TYPENAME);}

"const"                             {return(CONST);}
"ref"                               {return(REF);}

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

"clone"                             {return(CLONE);}
"length"                            {return(LENGTH);}

"namespace"                         {return NAMESPACE;}
"enum"                              {return ENUM;}
"struct"                            {return STRUCT;}
"function"                          {return FUNCTION;}

"print"                             {return PRINT;}
"readline"                          {return READLINE;}
"readchar"                          {return READCHAR;}
"read"                              {return READ;}

"for"                               {return FOR;}
"if"                                {return IF;}
"else"                              {return ELSE;}
"do"                                {return DO;}
"while"                             {return WHILE;}
"switch_value"                      {return SWITCH_VALUE;}
"switch"                            {return SWITCH;}
"when"                              {return WHEN;}
"default"                           {return DEFAULT;}
"try"                               {return TRY;}
"catch"                             {return CATCH;}
"throw"                             {return THROW;}
"return"                            {return RETURN;}
"break"                             {return BREAK;}

":"                                 {return COLON;}
";"                                 {return SEMICOLON;}
","                                 {return COMMA;}
"."                                 {return DOT;}
\"(\\.|[^"])*\"                     {yylval.sValue = strdup(yytext); return STRING_LITERAL;}
[a-zA-Z_][a-zA-Z0-9_]*                {yylval.sValue = strdup(yytext); return ID;}

. yyerror("invalid character");

%%

#ifndef yywrap
   int yywrap (void) {return 1;}
#endif
