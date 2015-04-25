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

"+"                                 {return(PLUS);}
"-"                                 {return(MINUS);}
"*"                                 {return(TIMES);}
"/"                                 {return(DIVIDE);}
"%"                                 {return(MOD);}

"enum"                              {return ENUM;}
"int"                               {return INT;}
"print"                             {return PRINT;}
"return"                            {return RETURN;}
";"                                 {return SEMICOLON;}
","                                 {return COMMA;}
\"(\\.|[^"])*\"                     {yylval.sValue = strdup(yytext); return STRING;}
[a-zA-Z][[a-zA-Z0-9]*               {yylval.sValue = strdup(yytext); return ID;}

. yyerror("invalid character");

%%

#ifndef yywrap
   int yywrap (void) {return 1;}
#endif
