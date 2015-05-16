%{
#include <stdio.h>

#include <string.h> //strlen, strcat, ...
#include <stdlib.h> //malloc

#include "squirrel.h"
#include "hashtable.h"
#include "arraylist.h"

hashtable * symbolTable;

char * varToString(TableRow * row){
    const char * constStr = "";
    if(row->value.variableValue.isConst){
        constStr = "const ";
    }
    TableRow * type = row->value.variableValue.type;
    const char * typename = (type == NULL) ? "<?>" : type->name;
    return concat3(constStr, typename, " variable");
}

char * tableRowToString(TableRow * row){
    switch(row->category){
        case categ_primitiveType:
            return cpyString("primitive");
        case categ_structType:
            return cpyString("struct");
        case categ_functionType:
            return cpyString("function type");
        case categ_enumType:
            return cpyString("enum");
        case categ_function:
            return cpyString("function");
        case categ_variable:
            return varToString(row);
        case categ_namespace:
            return cpyString("namespace");
        default:
            return NULL;
    }
    
}

void dumpRow(char * key, void * value){
    printf("%s \t-->\t ", key);
    if(value == NULL){
        printf("NULL");
    }
    else{
        TableRow * row = (TableRow *)value;
        char * rowStr = tableRowToString(row);
        if(rowStr != NULL){
            printf("%s",rowStr);
            free(rowStr);
        }
    }
    
    printf("\n");
}
void dumpSymbolTable(){
    printf("\n\n-----------------HASHTABLE-----------------\n");
    if(symbolTable != NULL){
        int i;
        for(i=0; i < symbolTable->capacity; ++i){
            char * key = symbolTable->body[i].key;
            if(key != NULL){
                void * value = symbolTable->body[i].value;
                dumpRow(key, value);
            }
        } 
    
    }
}


%}

%union {
    char * sValue;  /* string value */
    Expression * eValue;
    NameList * namesValue;
    NameDeclItem * nameDeclValue;    
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

%type <sValue> block_body block_stmt_list struct_body 
%type <sValue> for_statement for_header for_expr
%type <sValue> switch_header

%type <sValue> function_call lvalue_call io_command

%type <sValue> func_params function
%type <sValue> param_decl_list param_decl type_decl

%type <eValue> expr_list

%type <sValue> expr
%type <sValue> binary_expr
%type <sValue> unary_pre_expr unary_pos_expr
%type <sValue> term rvalue_term value

%type <sValue> operator assignment_op inc_op unary_pre_op

%type <sValue> attribute_list variables_decl
%type <sValue> type_modifier_list type_modifier
%type <sValue> type simple_type array_type primitive_type

%type <sValue> id_list

%type <sValue> struct_constructor member_init member_init_list
%type <sValue> member

%type <sValue> for if while do_while try_catch switch switch_body when_list when_block default_block conditional_test if_block

%type <sValue> inc_stmt lvalue_term clone_expr length_expr slice_expr opt_expr
%type <sValue> call_expr
%type <sValue> array_literal index_access 
%type <sValue> type_or_expr type_expr

%type <namesValue> name_decl_list
%type <nameDeclValue> name_decl
//%type <sValue> name_decl_list name_decl

%start program

%%
program          : declaration_list                 { printf("%s\n", $1); dumpSymbolTable();};

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

param_decl      : type_decl ID                              {   $$ = concat3($1," ",$2);};

/* ************************************* TYPES *********************************************** */

type_decl       : type                              {   $$ = $1;}
                    | type_modifier_list type       {   $$ = concat3($1, " ", $2);};

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

/* ********************************* TYPE DEFINITION ***************************************** */

enum_definition   : ENUM ID LBRACE id_list RBRACE   {   const char * values[] = {"enum ", $2, "{", $4, "}"};
                                                        $$ = concat_n(5, values);};
                                                        
struct_definition : STRUCT ID 
                    LBRACE struct_body RBRACE       {   const char * values[] = {"struct ", $2, "{\n", $4 , "\n}"};
                                                        $$ = concat_n(5, values);};
struct_body       : /*Vazio*/            {  $$ = "";}
                        | attribute_list {  $$ = $1;};
                                                        
functiontype_definition: 
                    FUNCTION type ID func_params    {   const char * values[] = {"function ", $2," ", $3, $4};
                                                        $$ = concat_n(5, values);};

id_list : ID                                        {   $$ = $1;}
            | id_list COMMA  ID                     {   $$ = concat3($1, ",", $3);};
            
attribute_list  :  variables_decl SEMICOLON                     {   $$ = concat($1,";"); }
                    | attribute_list variables_decl SEMICOLON   {   $$ = concat4($1, "\n", $2, ";");};

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
                      |io_command                                {   $$ = $1;};
                    //| rvalue_term LPAREN expr_list RPAREN       {   $$ = concat4($1, "(", $3, ")");};
                    
lvalue_call      : lvalue_term LPAREN expr /*expr_list*/ RPAREN               {   const char * values[] = {$1, "(", $3, ")"};
                                                                    $$ = concat_n(4, values); };

io_command       : PRINT LPAREN expr_list RPAREN              {   $$ = concat3("printf(\"%s\",", sq_exprToStr($3), ")");}
                    | READ LPAREN expr RPAREN                 {   $$ = concat(concat("read(", $3), ")"); }
                    | READCHAR LPAREN RPAREN                  {   $$ = strdup("readchar()"); }
                    | READLINE LPAREN RPAREN                  {   $$ = strdup("readline()"); };


return_statement : RETURN expr                                  {   $$ = concat("return ", $2); }
                   | RETURN                                     {   $$ = strdup("return ");};

assignment       : lvalue_term assignment_op expr               {   void * lValueTerm = hashtable_get(symbolTable, $1);
                                                                    if(lValueTerm != NULL){
                                                                        printf("On assignment, found var: %s \n", $1);
                                                                    }
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

type_modifier_list : type_modifier                              {   $$ = $1; }
                       | type_modifier_list type_modifier       {   const char *values[] = {$1, " ", $2}; 
                                                                    $$ = concat_n(3, values);};

type_modifier : CONST { $$ = strdup("const");}
                | REF { $$ = strdup("ref");};

/* ********************************* EXPRESSIONS ********************************************* */
expr_list       : /* Vazio */                                   {   $$ = NULL;}
                    | NUMBER /*expr*/                           {   $$ = sq_Expression(type_int, $1);};
                    //| expr_list COMMA NUMBER/*expr*/            {   $$ = concat(concat($1, ","), $3);};

expr            : binary_expr                       {   $$ = $1;}
                    | type_expr                     {   $$ = $1;};

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
