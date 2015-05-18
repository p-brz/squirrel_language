%{
#include <stdio.h>

#include <string.h> //strlen, strcat, ...
#include <stdlib.h> //malloc

#include "squirrel.h"
#include "dump_table.h"

hashtable * symbolTable;


char * AttributeDeclStringConverter(void * value){
    if(value == NULL){
        return cpyString("");
    }
    AttributeDecl * attributeDecl = (AttributeDecl *)value;
    char * nameDeclListStr = joinList(attributeDecl->namesList, ", ", NULL);
    char * result = concat3(attributeDecl->type, " ", nameDeclListStr);
    free(nameDeclListStr);
    return result;
}

char * attributeListToString(AttributeList * attributeList){
    char * listStr =  joinList(attributeList, ";\n", AttributeDeclStringConverter);
    char * result = concat(listStr, ";");//último ';' não é adicionado por joinList
    free(listStr);
    return result;
}

%}

%union {
    char             * sValue;  /* string value */
    Expression       * eValue;
    NameDeclList     * NameDeclList_Value;
    NameDeclItem     * nameDeclValue;
    NameList         * NameList_Value; 
    Parameter        * ParamValue; 
    ParamList        * ParamListValue;
    AttributeDecl    * AttributeDeclValue;
    AttributeList    * AttributeListValue;
};

%token <sValue> ID
%token <sValue> NUMBER
%token <sValue> REAL_LITERAL BOOLEAN_LITERAL
%token <sValue> STRING_LITERAL 

%token LPAREN RPAREN LBRACE RBRACE LBRACKET RBRACKET
%token SEMICOLON COMMA COLON DOT
%token ARRAY_SYMBOL NEW

%token ENUM STRUCT FUNCTION
%token NAMESPACE

%token PRINT READ READCHAR READLINE

%token PLUS MINUS TIMES DIVIDE MOD OU OR BITOR BITAND EE AND PLUSPLUS MINUSMINUS SHIFTL SHIFTR EQUAL DIFERENT MINOR BIGGER MINOREQUAL BIGGEREQUAL EXCLAMATION TIO NOT

%token ASSIGN ASSIGNPLUS ASSIGNMINUS ASSIGNDIVIDE ASSIGNMOD ASSIGNBITOR ASSIGNBITAND

%token CONST REF

%token VOID BYTE SHORT INT LONG FLOAT DOUBLE BOOLEAN STRING OBJECT TYPE

%token TYPEOF TYPENAME CASTSTO

%token CLONE LENGTH

%token FOR IF ELSE WHILE DO WHEN TRY CATCH SWITCH SWITCH_VALUE DEFAULT 
%token RETURN BREAK THROW

%type <sValue> declaration declaration_list
%type <sValue> namespace type_definition enum_definition struct_definition functiontype_definition
%type <sValue> statement_list statement inline_statement std_statement block_statement 
%type <sValue> assignment return_statement

%type <sValue> block_body block_stmt_list
%type <sValue> for_statement for_header for_expr
%type <sValue> switch_header

%type <sValue> function_call lvalue_call io_command

%type <sValue> function
%type <ParamListValue> func_params param_decl_list
%type <ParamValue> param_decl

//%type <eValue> expr_list
%type <sValue> expr_list

%type <sValue> expr
%type <sValue> binary_expr
%type <sValue> unary_pre_expr unary_pos_expr
%type <sValue> term rvalue_term value

%type <sValue> operator assignment_op inc_op unary_pre_op

%type <sValue> variables_decl
%type <sValue> type simple_type array_type primitive_type

%type <AttributeListValue> attribute_list struct_body
%type <AttributeDeclValue> attribute

%type <sValue> type_modifier
%type <NameList_Value> type_modifier_list

%type <NameList_Value> id_list

%type <sValue> struct_constructor member_init member_init_list
%type <sValue> member

%type <sValue> for if while do_while try_catch switch switch_body when_list when_block default_block conditional_test if_block

%type <sValue> inc_stmt lvalue_term clone_expr length_expr slice_expr opt_expr
%type <sValue> call_expr
%type <sValue> array_literal index_access 
%type <sValue> type_or_expr type_expr

%type <NameDeclList_Value> name_decl_list
%type <nameDeclValue> name_decl

%start program

%%
program          : declaration_list                 { printf("%s\n", $1); dumpSymbolTable(symbolTable);};

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
                    LBRACE declaration_list RBRACE  {   sq_declareNamespace(symbolTable, $2);
                                                        const char * values[] = {"namespace ", $2, "{\n", $4, "\n}"};
                                                        $$ = concat_n(5, values); };
                                                        
    
/* *********************************** FUNCTIONS ********************************************** */

function        : type ID func_params block_body    {   char * funcParams = joinList($3, ", ", sq_ParameterToString);
                                                        sq_declareFunction(symbolTable, $1, $2, $3);
                                                        
                                                        const char * values[] = {$1, " ", $2, "(", funcParams, ")", $4};
                                                        $$ = concat_n(7, values); 
                                                        free(funcParams);
                                                        //TODO: destruir Parameter list
                                                    };

func_params     : LPAREN param_decl_list RPAREN     {   $$ = $2; };

param_decl_list : /* Vazio*/                                {   $$ = createList(NULL);}
                      | param_decl                          {   $$ = createList($1);}
                      | param_decl_list COMMA param_decl    {   $$ = appendList($1, $3);};

param_decl      : type ID                                   {   $$ = sq_Parameter($1, $2, createList(NULL));}
                    | type_modifier_list type ID            {   $$ = sq_Parameter($2, $3, $1);};

/* ************************************* TYPES *********************************************** */

type            : simple_type                       {   $$ = $1; }
                    | array_type                    {   $$ = $1; };

simple_type : primitive_type    { $$ = $1; }
                | member          { $$ = $1; };
              
primitive_type : VOID       { $$ = strdup("void"); }
                  | BYTE    { $$ = strdup("byte"); }
                  | SHORT   { $$ = strdup("short"); }
                  | INT     { $$ = strdup("int");}
                  | LONG    { $$ = strdup("long"); } 
                  | FLOAT   { $$ = strdup("float"); }
                  | DOUBLE  { $$ = strdup("double"); }
                  | BOOLEAN { $$ = strdup("boolean"); }
                  | STRING  { $$ = strdup("string"); }
                  | OBJECT  { $$ = strdup("object"); }
                  | TYPE    { $$ = strdup("type"); };

array_type   : type ARRAY_SYMBOL       {   $$ = concat($1, "[]"); };

type_modifier_list : type_modifier                              {   $$ = createList($1); }
                       | type_modifier_list type_modifier       {   $$ = appendList($1, $2);};

type_modifier : CONST { $$ = strdup("const");}
                | REF { $$ = strdup("ref");};
                
/* ********************************* TYPE DEFINITION ***************************************** */

enum_definition   : ENUM ID LBRACE id_list RBRACE   {   sq_declareEnum(symbolTable, $2, $4);

                                                        char * listStr = joinList($4, ", ", NULL);
                                                        const char * values[] = {"enum ", $2, "{", listStr, "}"};
                                                        $$ = concat_n(5, values);
                                                        free(listStr);};
                                                        
struct_definition : STRUCT ID 
                    LBRACE struct_body RBRACE       {   sq_declareStruct(symbolTable, $2, $4);
                    
                                                        char * attrListStr = attributeListToString($4);
                                                        const char * values[] = {"struct ", $2, "{\n", attrListStr , "\n}"};
                                                        $$ = concat_n(5, values);
                                                        free(attrListStr);};
struct_body       : /*Vazio*/            {  $$ = createList(NULL);}
                        | attribute_list {  $$ = $1;};
                                                        
functiontype_definition: 
                    FUNCTION type ID func_params    {   sq_declareFunctionType(symbolTable, $2,$3,$4);
                                                        
                                                        char * funcParams = joinList($4, ", ", sq_ParameterToString);
                                                        const char * values[] = {"function ", $2," ", $3, "(",funcParams,")"};
                                                        $$ = concat_n(7, values);
                                                        free(funcParams);};

id_list : ID                                        {   $$ = createList($1);}
            | id_list COMMA  ID                     {   $$ = appendList($1, $3);};
            
attribute_list  :  attribute                        {   $$ = createList($1);}
                                                        //char * listStr = joinList($1->nameDeclList,", ", sq_NameDeclToString);
                                                        //$$ = concat4($1->type," ", listStr, ";\n");
                                                        //free(listStr); }
                    | attribute_list attribute      {   $$ = appendList($1, $2);};
                                                        //char * listStr = joinList($2->nameDeclList,", ", sq_NameDeclToString);
                                                        //char * attribute = concat4($2->type," ", listStr, ";\n");
                                                        //free(listStr); 
                                                        //$$ = concat($1, attribute);};
                    
attribute       : type id_list SEMICOLON            {   $$ = sq_AttributeDecl($1, $2);};
                    
//attribute_list  :  variables_decl SEMICOLON                     {   $$ = concat($1,";"); }
//                    | attribute_list variables_decl SEMICOLON   {   $$ = concat4($1, "\n", $2, ";");};

struct_constructor  : member LBRACE member_init_list RBRACE         {   const char * valores[] = {$1, "{", $3, "}"};
                                                                    $$ = concat_n(4, valores);};

member_init_list    :   /*vazio*/                               {   $$ = "";}
                        | member_init                           {   $$ = $1;} 
                        | member_init_list COMMA member_init    {   $$ = concat3($1, ", ", $3);};

member_init         : ID COLON expr                             {   $$ = concat3($1, " : ", $3);};

/* *********************************** STATEMENTS ******************************************** */
block_body       : LBRACE block_stmt_list RBRACE                {   char * begin_block = concat("{\n", $2);
                                                                    $$ = concat(begin_block, "}\n"); };

block_stmt_list  : /* Vazio */                                  {   $$ = ""; }
                    | statement_list                            {   $$ = $1; };

statement_list  : statement	                                    {   $$ = concat($1, "\n");}
                    | statement_list statement		            {   $$ = concat3($1,$2,"\n"); };
                                                                    
statement       : block_statement                               {   $$ = $1;}
		            | inline_statement SEMICOLON                {   $$ = concat3("\t",$1, ";");};

inline_statement : std_statement                                {   $$ = $1;}
                    | return_statement                          {   $$ = $1; }
                    | THROW                                     {   $$ = strdup("throw"); }
                    | BREAK                                     {   $$ = strdup("break"); };

std_statement    : function_call                                {   $$ = $1; }
                    | variables_decl                            {   $$ = $1; }
                    | assignment                                {   $$ = $1; }
                    | inc_stmt                                  {   $$ = $1; };

inc_stmt         : lvalue_term inc_op { $$ = concat($1, $2);}
                    | inc_op lvalue_term { $$ = concat($1, $2);};

function_call    : lvalue_call                                  {   $$ = $1;}
                      |io_command                                {   $$ = $1;}
                    | rvalue_term LPAREN expr_list RPAREN       {   $$ = concat4($1, "(", $3, ")");};
                    
lvalue_call      : lvalue_term LPAREN expr_list RPAREN          {   $$ = concat4($1, "(", $3, ")"); };

//io_command       : PRINT LPAREN expr_list RPAREN              {   $$ = concat3("printf(\"%s\",", sq_exprToStr($3), ")");}
io_command       : PRINT LPAREN expr_list RPAREN              {   $$ = concat3("printf(\"%s\",", $3, ")");}
                    | READ LPAREN expr RPAREN                 {   $$ = concat(concat("read(", $3), ")"); }
                    | READCHAR LPAREN RPAREN                  {   $$ = strdup("readchar()"); }
                    | READLINE LPAREN RPAREN                  {   $$ = strdup("readline()"); };


return_statement : RETURN expr                                  {   $$ = concat("return ", $2); }
                   | RETURN                                     {   $$ = strdup("return ");};

assignment       : lvalue_term assignment_op expr               {  
                                                                    const char *values[] = {$1, " ", $2, " ", $3};
                                                                    $$ = concat_n(5, values);
                                                                };

variables_decl   : type name_decl_list                          {   
                                                                    sq_declareVariables(symbolTable, $1, $2);
                                                                    $$ = concat3($1, " ", joinList($2,", ", sq_NameDeclToString));
                                                                    destroyList($2);
                                                                }
                                                                
                      /* Assumindo que variáveis podem ter apenas modificador CONST*/
                    | CONST type name_decl_list                 {   
                                                                     sq_declareConstants(symbolTable, $2, $3); 
                                                                     char * listStr = joinList($3, ", ", sq_NameDeclToString);
                                                                     $$ = concat4("const ", $2, " ", listStr);
                                                                     free(listStr); 
                                                                     destroyList($3);   
                                                                };

name_decl_list   : name_decl                                    {   $$ = createList($1);}
                    | name_decl_list COMMA name_decl            {   $$ = appendList($1, $3);};

name_decl         : ID                                          {   $$ = sq_NameDeclItem($1, NULL); }
                    | ID ASSIGN expr                            {   $$ = sq_NameDeclItem($1, sq_Expression(type_void, $3));
                                                                    //const char *values[] = {$1, " = ", $3}; 
                                                                    //$$ = concat_n(3, values);
                                                                    };

/* ********************************* EXPRESSIONS ********************************************* */
expr_list       : /* Vazio */                                   {   $$ = "";}
                    //| NUMBER /*expr*/                           {   $$ = sq_Expression(type_int, $1);};
                    | expr                           {   $$ = $1;};
                    | expr_list COMMA expr            {   $$ = concat3($1, ",", $3);};

expr            : binary_expr                                 {   $$ = $1; }
                    | type_expr                               {   $$ = $1; };

binary_expr     : unary_pos_expr                              {   $$ = $1;}
                    | binary_expr operator unary_pos_expr     {   const char * values[] = {$1, " ", $2, " ", $3};
                                                                  $$ = concat_n(5, values);};
                                                                  
type_expr       : TYPEOF LPAREN type_or_expr RPAREN           {   $$ = concat3("typeof(", $3, ")");}
                    | TYPENAME LPAREN type_or_expr RPAREN     {   $$ = concat3("typename(", $3, ")");}
                    | type_or_expr 
                        CASTSTO LPAREN type_or_expr RPAREN    {   const char * values[] = {$1, " caststo(", $4, ")"};
                                                                  $$ = concat_n(4, values);};

type_or_expr    :   expr                                      {   $$ = $1;}
                    | array_type                              {   $$ = $1;}
                    | primitive_type                          {   $$ = $1;};

unary_pos_expr  : unary_pre_expr                    {   $$ = $1; }
                    | unary_pre_expr inc_op         {   $$ = concat($1, $2);};

unary_pre_expr  : term                              {   $$ = $1; }
                    | unary_pre_op term             {   $$ = concat($1, $2); };
                   
term            : rvalue_term                       {   $$ = $1;}
                    | lvalue_term                   {   $$ = $1;};
                   
rvalue_term     :   LPAREN expr RPAREN              {   $$ = concat3("(",$2,")");}
                    | call_expr                     {   $$ = $1;}
                    | struct_constructor            {   $$ = $1;}
                    | value                         {   $$ = $1;}
                    | clone_expr                    {   $$ = $1;}
                    | length_expr                   {   $$ = $1;}
                    | slice_expr                    {   $$ = $1;};

call_expr       :   function_call                               {   $$ = $1; }
                        /* Casts*/
                        | primitive_type LPAREN expr RPAREN     {   const char * values[] = {$1, "(", $3, ")"};
                                                                    $$ = concat_n(4, values); }
                        | array_type LPAREN expr RPAREN         {   const char * values[] = {$1, "(", $3, ")"};
                                                                    $$ = concat_n(4, values); };

clone_expr      : CLONE LPAREN expr RPAREN                      {   const char * values[] = {"clone", "(", $3, ")"};
                                                                    $$ = concat_n(4, values); };

length_expr     :  member DOT LENGTH                            {   $$ = concat($1,".length");};

slice_expr      : term 
                    LBRACKET opt_expr COLON opt_expr RBRACKET   {   const char * values[] = {$1, "[ ", $3, " : ", $5, " ]"};
                                                                    $$ = concat_n(6, values);};
opt_expr        : /*Vazio*/   { $$ = "";}
                    | expr  { $$ = $1;};
                                                                                       
lvalue_term     :  member                           { $$ = $1;}
                    | index_access                  { $$ = $1;}
                    | rvalue_term DOT member        { $$ = concat3($1, ".", $3);}
                    | index_access DOT member       { $$ = concat3($1, ".", $3);};

index_access    : term LBRACKET expr RBRACKET       {   const char * values[] = {$1, "[", $3, "]"};
                                                        $$ = concat_n(4, values);};

value           : NUMBER                            {   $$ = strdup($1); } 
                    | REAL_LITERAL                  {   $$ = strdup($1); }
                    | STRING_LITERAL                {   $$ = strdup($1); }
                    | BOOLEAN_LITERAL               {   $$ = strdup($1); }
                    | array_literal                 {   $$ = $1; }
                    | SWITCH_VALUE                  {   $$ = strdup("switch_value");};
                    
array_literal   : ARRAY_SYMBOL                      {   $$ = strdup("[]");}
                    //| LBRACKET expr_list RBRACKET   {   $$ = concat3("[", $2, "]");}
                    | NEW type 
                        LBRACKET expr RBRACKET      {   const char * values[] = {"new ", $2, "[", $4, "]"};
                                                        $$ = concat_n(5, values);};

member          : ID                                {   $$ = $1;}
                    | member DOT ID                 {   $$ = concat3($1,".",$3);};

/* ********************************* OPERATORS ********************************************* */

assignment_op   : ASSIGN                { $$ = strdup("=");}
                    |ASSIGNPLUS         { $$ = strdup("+=");}
                    |ASSIGNMINUS        { $$ = strdup("-=");}
                    |ASSIGNDIVIDE       { $$ = strdup("/=");}
                    |ASSIGNMOD          { $$ = strdup("%=");}
                    |ASSIGNBITOR        { $$ = strdup("|=");}
                    |ASSIGNBITAND       { $$ = strdup("&=");};

inc_op : PLUSPLUS       { $$ = strdup("++");}
         | MINUSMINUS   { $$ = strdup("--");};

unary_pre_op : inc_op { $$ = $1; }
	            |EXCLAMATION	{ $$ = strdup("!");}
	            |NOT 		{ $$ = strdup("not ");}
 	            |TIO		{ $$ = strdup("~");}
	            |MINUS 		{ $$ = strdup("-");};

operator        :   PLUS                            { $$ = strdup("+");}
                    | MINUS                         { $$ = strdup("-");}
                    | TIMES                         { $$ = strdup("*");}
                    | DIVIDE                        { $$ = strdup("/");}
                    | MOD                           { $$ = strdup("%");}
                    | OU                            { $$ = strdup("||");}
                    | OR                            { $$ = strdup("or");}
                    | BITOR                         { $$ = strdup("|");}
                    | BITAND                        { $$ = strdup("&");}
                    | EE                            { $$ = strdup("&&");}
                    | AND                           { $$ = strdup("and");}
                    | SHIFTL                        { $$ = strdup("<<");}
                    | SHIFTR                        { $$ = strdup(">>");}
                    | EQUAL                         { $$ = strdup("==");}
                    | DIFERENT                      { $$ = strdup("!=");}
                    | MINOR                         { $$ = strdup("<");}
                    | BIGGER                        { $$ = strdup(">");}
                    | MINOREQUAL                    { $$ = strdup("<=");}
                    | BIGGEREQUAL                   { $$ = strdup(">=");};


                 /* ********************** BLOCK STATEMENTS *********************** */

block_statement : for       		{$$ = $1;}
			        | if       		{$$ = $1;}
			        | while         {$$ = $1;}
			        | do_while      {$$ = $1;} 
			        | try_catch     {$$ = $1;}
			        | switch        {$$ = $1;};


for             : FOR for_header block_body     {$$ = concat3("for", $2, $3);};
                                                        					
for_header      : LPAREN 
                    for_statement SEMICOLON 
                    for_expr      SEMICOLON 
                    for_statement 
                  RPAREN                        {   const char * values[] = {"(",$2,";", $4, ";",$6 ,")"};
                                                    $$ = concat_n(7, values);};
                                        
for_statement   : /*Vazio*/                     {$$ = "";}
			        | std_statement             {$$ = $1;};

for_expr        : /*Vazio*/                     {$$ = " ";}
			        | binary_expr               {$$ = $1;};

if 		        : if_block				        { $$ = $1;}
    		        | if_block ELSE block_body	{ $$ = concat3($1, "\nelse ", $3);}
   			        | if_block ELSE if		    { $$ = concat3($1, "\nelse ", $3);};

if_block 	    : IF conditional_test block_body 	        {   $$ = concat3("if", $2, $3);};

while 		    : WHILE conditional_test block_body         {   $$ = concat3("while", $2, $3);};

do_while        : DO block_body 
                    WHILE conditional_test SEMICOLON        {   const char * values[] = {"do",$2," while", $4, ";"};
                                                                $$ = concat_n(5, values);};

try_catch 	    : TRY block_body CATCH block_body           {   $$ = concat4("try",$2,"catch", $4);};

switch          : switch_header LBRACE switch_body RBRACE   {   $$ = concat4($1, "{\n", $3, "\n}");};

switch_header   : SWITCH LPAREN opt_expr RPAREN             {   $$ = concat3("switch(", $3, ")");};

switch_body 	: when_list				                    {   $$ = $1;}
		            | when_list default_block		        {   $$ = concat($1, $2);};

when_list       : when_block				                {   $$ = $1;}
	                | when_list when_block			        {   $$ = concat($1, $2);};

when_block 	    : WHEN conditional_test block_body          {   $$ = concat3("when", $2, $3);};

default_block   : DEFAULT block_body                        {   $$ = concat("default", $2);};

conditional_test: LPAREN expr RPAREN			            {   $$ = concat3("(", $2, ")");};
%%

int main (void) {
    symbolTable = sq_createSymbolTable();
    
    int exitCode =  yyparse ( );
    
    hashtable_destroy(symbolTable);
    symbolTable = NULL;
    
    return exitCode;
}

int yyerror (char *s) {fprintf (stderr, "%s\n", s);}

