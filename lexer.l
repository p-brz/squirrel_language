%{
  #include "y.tab.h"
%}

%%

[ \t\n]                             ;
[0-9]+                              {yylval.iValue = atoi(yytext); return NUMBER;}
"int"                               {return INT;}
"("                                 {return LPAREN;}
")"                                 {return RPAREN;}
"{"                                 {return LBRACE;}
"}"                                 {return RBRACE;}
"print"                             {return PRINT;}
"return"                            {return RETURN;}
";"                                 {return SEMICOLON;}
\"(\\.|[^"])*\"                     {yylval.sValue = strcpy(yylval.sValue, yytext); return STRING;}
[a-zA-Z][[a-zA-Z0-9]*               {yylval.sValue = yytext; return ID;}

. yyerror("invalid character");

%%

#ifndef yywrap
   int yywrap (void) {return 1;}
#endif
