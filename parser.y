%{
#include <stdio.h>

#include <string.h> //strlen, strcat, ...
#include <stdlib.h> //malloc

#include "squirrel.h"
#include "dump_table.h"

SquirrelContext * sqContext;
//Saída padrão é a.out
char * output_file = "out.c";

void startScope(const char * scopeName){
    sq_startScope(sqContext, scopeName);
}

void finishScope(){
    sq_finishScope(sqContext);
}
%}

%union {
    char             * sValue;  /* string value */
    Expression       * eValue;
    ExpressionList   * ExpressionListValue;
    NameDeclList     * NameDeclList_Value;
    NameDeclItem     * nameDeclValue;
    NameList         * NameList_Value; 
    Parameter        * ParamValue; 
    ParamList        * ParamListValue;
    AttributeDecl    * AttributeDeclValue;
    AttributeList    * AttributeListValue;
    Member           * memberValue;
    IfStruct         * ifValue;
};

%token <sValue> ID
%token <sValue> NUMBER_LITERAL
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
%type <sValue> for_statement for_expr

%type <NameList_Value> for_header

%type <eValue> function_call
%type <eValue> lvalue_call io_command

%type <sValue> function
%type <ParamListValue> func_params param_decl_list
%type <ParamValue> param_decl

//%type <eValue> expr_list
%type <ExpressionListValue> expr_list

%type <eValue> expr
%type <eValue> binary_expr
%type <eValue> unary_pre_expr unary_pos_expr
%type <eValue> term
%type <eValue> rvalue_term 
%type <eValue> value length_expr

%type <sValue> operator assignment_op inc_op unary_pre_op

%type <sValue> variables_decl
%type <sValue> type simple_type array_type primitive_type 

%type <AttributeListValue> attribute_list struct_body
%type <AttributeDeclValue> attribute

%type <sValue> type_modifier
%type <NameList_Value> type_modifier_list

%type <NameList_Value> id_list

%type <sValue> struct_constructor member_init member_init_list
%type <memberValue> member

%type <sValue> for while do_while try_catch conditional_test
%type <sValue> if else_block
%type <ifValue> if_block


%type <eValue> lvalue_term
%type <sValue> inc_stmt clone_expr  slice_expr opt_expr
%type <eValue> call_expr
%type <eValue> index_access 
%type <eValue> array_literal 
%type <eValue> type_or_expr 
%type <eValue> type_expr

%type <NameDeclList_Value> name_decl_list
%type <nameDeclValue> name_decl

%start program

%%
program          : declaration_list                 { 
                                                        char * programOut = gen_program(sqContext, $1);
                                                        printf("------------------START PROGRAM----------------\n"); 
                                                        printf("%s\n", programOut); 
                                                        dumpSymbolTable(sqContext->symbolTable);
                                                        if(sq_hasErrors(sqContext)){
                                                            sq_printStatus(sqContext);
                                                        }
                                                        else{
                                                            sq_writeFile(programOut, output_file);  
                                                        }
                                                    };

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
                      
namespace        : NAMESPACE ID {sq_declareNamespace(sqContext, $2); startScope($2);}
                    LBRACE declaration_list RBRACE  {   
                                                        const char * values[] = {"namespace ", $2, "{\n", $5, "\n}"};
                                                        $$ = concat_n(5, values);
                                                        
                                                        finishScope(); };
                                                        
    
/* *********************************** FUNCTIONS ********************************************** */

function        : type ID func_params               {   //Declare function and start scope
                                                        sq_startFunction(sqContext, $1, $2, $3);}
                                    block_body      {   //NOTE: gerando parcialmente código em função
                                                        $$ = sq_genFunction(sqContext, $1, $2, $3, $5);
                                                        //TODO: destruir Parameter list
                                                        
                                                        finishScope(); //scope created in function
                                                    };

func_params     : LPAREN param_decl_list RPAREN             {   $$ = $2; };

param_decl_list : /* Vazio*/                                {   $$ = createList(NULL);}
                      | param_decl                          {   $$ = createList($1);}
                      | param_decl_list COMMA param_decl    {   $$ = appendList($1, $3);};

param_decl      : type ID                                   {   $$ = sq_Parameter($1, $2, createList(NULL));}
                    | type_modifier_list type ID            {   $$ = sq_Parameter($2, $3, $1);};

/* ************************************* TYPES *********************************************** */

type            : simple_type                       {   $$ = $1; }
                    | array_type                    {   $$ = $1; };

simple_type : primitive_type                        { $$ = $1; }
                | member                            { $$ = sq_memberToString($1); };
              
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

array_type   : type ARRAY_SYMBOL                                {   sq_declareArrayType(sqContext, $1);
                                                                    $$ = concat($1, "[]"); };

type_modifier_list : type_modifier                              {   $$ = createList($1); }
                       | type_modifier_list type_modifier       {   $$ = appendList($1, $2);};

type_modifier : CONST { $$ = strdup("const");}
                | REF { $$ = strdup("ref");};
                
/* ********************************* TYPE DEFINITION ***************************************** */

enum_definition   : ENUM ID LBRACE id_list RBRACE   {   sq_declareEnum(sqContext, $2, $4);
                                                        
                                                      //  char * listStr = joinList($4, ", ", NULL);
                                                        $$ = sq_genEnum( sqContext, $2, $4);
                                                        
                                                       };
                                                        
struct_definition : STRUCT ID 
                    LBRACE struct_body RBRACE       {   
                                                        sq_declareStruct(sqContext, $2, $4);
                                                        $$ = sq_genStruct(sqContext, $2, $4);/*sq_genStructDefinition(sqContext, $2, $4)*/
                                                    }
                                                        
struct_body       : /*Vazio*/                       {  $$ = createList(NULL); }
                        | attribute_list            {  $$ = $1;};
                                                        
functiontype_definition: 
                    FUNCTION type ID func_params    {   sq_declareFunctionType(sqContext, $2,$3,$4);
                                                        $$ = sq_genFunctionType(sqContext,$2,$3,$4);};

id_list : ID                                        {   $$ = createList($1);}
            | id_list COMMA  ID                     {   $$ = appendList($1, $3);};
            
attribute_list  :  attribute                        {   $$ = createList($1); }
                    | attribute_list attribute      {   $$ = appendList($1, $2);};
                    
attribute       : type id_list SEMICOLON            {  $$ = sq_AttributeDecl($1, $2);};
                    
struct_constructor  : member LBRACE member_init_list RBRACE     {   const char * valores[] = {sq_memberToString($1), "{", $3, "}"};
                                                                    $$ = concat_n(4, valores);};

member_init_list    :   /*vazio*/                               {   $$ = "";}
                        | member_init                           {   $$ = $1;} 
                        | member_init_list COMMA member_init    {   $$ = concat3($1, ", ", $3);};

member_init         : ID COLON expr                             {   $$ = concat3($1, " : ", sq_exprToStr($3));};

/* *********************************** STATEMENTS ******************************************** */
block_body       : LBRACE block_stmt_list RBRACE                {   $$ = $2;} //"{} vão ser inclusas em regras que usem block_body, quando necessario"

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

std_statement    : function_call                                {   $$ = sq_exprToStr($1); }
                    | variables_decl                            {   $$ = $1; }
                    | assignment                                {   $$ = $1; }
                    | inc_stmt                                  {   $$ = $1; };

inc_stmt         : lvalue_term inc_op                           { //TODO: Testar se lvalue_term é numérico e não é constante
                                                                    $$ = concat(sq_exprToStr($1), $2);}
                    | inc_op lvalue_term                        { //TODO: Testar se lvalue_term é numérico e não é constante
                                                                    $$ = concat($1, sq_exprToStr($2));};

function_call    : lvalue_call                                  {   $$ = $1;}
                    |io_command                                 {   $$ = $1;}
                    | rvalue_term LPAREN expr_list RPAREN       {   char * listStr = joinList($3, ", ", sq_ExpressionStringConverter);
                                                                    $$ = sq_Expression("void", concat4(sq_exprToStr($1), "(",listStr, ")"), type_invalid);};
                    
lvalue_call      : lvalue_term LPAREN expr_list RPAREN          {   
                                                                    
                                                                    if($1->typeCategory == type_typeliteral){
                                                                        if($3->size == 0){
                                                                            sq_putError(sqContext, "Cast without expression.");
                                                                            $$ = sq_Expression("", concat($1->expr, "()"), type_invalid);
                                                                        }
                                                                        else{
                                                                            if($3->size > 1){
                                                                                sq_putError(sqContext, "Trying to cast more than one expression.");
                                                                            } 
                                                                            
                                                                            Expression * expr = (Expression *)arraylist_get($3, 0);
                                                                            checkCastRule(sqContext, $1->type, $1->typeCategory, expr);
                                                                            char * castExpr = sq_genCastExpr(sqContext, $1->type, $1->typeCategory, expr);
                                                                            $$ = sq_Expression($1->type, castExpr, $1->typeCategory);
                                                                        }
                                                                    }
                                                                    else{
                                                                        char * listStr = joinList($3, ", ", sq_ExpressionStringConverter);
                                                                        char * translationExpr = concat4(sq_exprToStr($1), "(", listStr, ")");
                                                                        $$ = sq_Expression("error", translationExpr, type_invalid); 
                                                                    }
                                                                };

io_command       : PRINT LPAREN expr_list RPAREN              {   //char * listStr = joinList($3, ", ", sq_ExpressionStringConverter);
                                                                  //$$ = sq_Expression("void", concat3("print(", listStr, ")"), type_void);
                                                                  char * translationExpr = sq_genPrint(sqContext, $3) ;   
                                                                  $$ = sq_Expression("void", translationExpr, type_void);
                                                              }
                    | READ LPAREN expr RPAREN                 {   
                                                                checkCategoryEquals(sqContext, $3->typeCategory, type_integer);
                                                                $$ = sq_Expression("string",concat3("read(", sq_exprToStr($3), ")"), type_string); 
                                                              }
                                                              
                    | READCHAR LPAREN RPAREN                  {   $$ = sq_Expression("string", "readchar()", type_string); }
                    | READLINE LPAREN RPAREN                  {   $$ = sq_Expression("string", "readline()", type_string); };


return_statement : RETURN expr                                  {   $$ = concat("return ", sq_exprToStr($2)); }
                   | RETURN                                     {   $$ = strdup("return ");};

assignment       : lvalue_term assignment_op expr               {  
                                                                    const char *values[] = {sq_exprToStr($1), " ", $2, " ", sq_exprToStr($3)};
                                                                    $$ = concat_n(5, values);
                                                                };

variables_decl   : type name_decl_list                          {   
                                                                    sq_declareVariables(sqContext, $1, $2);
                                                                    char * typeName = sq_translateTypeName(sqContext,$1);
                                                                    printf("translated type %s to %s\n", $1, typeName);
                                                                    
                                                                    $$ = concat3(typeName, " ", joinList($2,", ", sq_NameDeclToString));
                                                                    destroyList($2);
                                                                    free(typeName);
                                                                }
                                                                
                      /* Assumindo que variáveis podem ter apenas modificador CONST*/
                    | CONST type name_decl_list                 {   
                                                                     sq_declareConstants(sqContext, $2, $3); 
                                                                     char * listStr = joinList($3, ", ", sq_NameDeclToString);
                                                                     $$ = concat4("const ", $2, " ", listStr);
                                                                     free(listStr); 
                                                                     destroyList($3);   
                                                                };

name_decl_list   : name_decl                                    {   $$ = createList($1);}
                    | name_decl_list COMMA name_decl            {   $$ = appendList($1, $3);};

name_decl         : ID                                          {   $$ = sq_NameDeclItem($1, NULL); }
                    | ID ASSIGN expr                            {   $$ = sq_NameDeclItem($1, sq_Expression("error", sq_exprToStr($3), type_invalid)); };

/* ********************************* EXPRESSIONS ********************************************* */
expr_list       : /* Vazio */                                   {   $$ = createList(NULL);}
                    | expr                                      {   $$ = createList($1);}
                    | expr_list COMMA expr                      {   $$ = appendList($1, $3);};

expr            : binary_expr                                 {   $$ = $1; }
                    | type_expr                               {   $$ = $1; };

binary_expr     : unary_pos_expr                              {   $$ = $1;}
                    | binary_expr operator unary_pos_expr     {   const char * values[] = {sq_exprToStr($1), " ", $2, " ", sq_exprToStr($3)};
                                                                  $$ = sq_Expression("error", concat_n(5, values), type_invalid);};
                                                                  
type_expr       : TYPEOF LPAREN type_or_expr RPAREN           {   
                                                                  const char * type_or_exprStr = $3->typeCategory == type_type ? $3->expr : $3->type;
                                                                  $$ = sq_Expression("type", sq_genTypeof(type_or_exprStr), type_type);
                                                              }
                    | TYPENAME LPAREN type_or_expr RPAREN     {     
                                                                    char * typenameStr = sq_genTypename(sqContext, $3);
                                                                    $$ = sq_Expression("string", typenameStr, type_string);
                                                                    free(typenameStr);
                                                              }
                    | type_or_expr 
                        CASTSTO LPAREN type_or_expr RPAREN    {   
                                                                  char * castsToExpr = sq_genCaststTo(sqContext, $1, $4);
                                                                  $$ = sq_Expression("boolean", castsToExpr, type_boolean);
                                                                  free(castsToExpr);
                                                              };

type_or_expr    :   expr                                      { $$ = $1; }
                    | array_type                              { $$ = sq_Expression($1, $1, type_typeliteral); }
                    | primitive_type                          { $$ = sq_Expression($1, $1, type_typeliteral); };

unary_pos_expr  : unary_pre_expr                    {   $$ = $1; }
                    | unary_pre_expr inc_op         {   
                                                        //TODO: checar se unary_pre_expr é algo que pode receber valor
                                                        char * exprTranslate = concat(sq_exprToStr($1), $2);
                                                        $$ = sq_Expression($1->type, exprTranslate, $1->typeCategory);
                                                    };

unary_pre_expr  : term                              {   $$ = $1; }
                    | unary_pre_op term             {  type typ = sq_getUnaryExpressionType(sqContext, $1,$2);
                                                        $$ = sq_Expression(typ.typename, concat($1, sq_exprToStr($2)),typ.category); };
                   
term            : rvalue_term                       {   $$ = $1;}
                    | lvalue_term                   {   $$ = $1;};
                   
rvalue_term     :   LPAREN expr RPAREN              {   $$ = sq_Expression("error", concat3("(",sq_exprToStr($2),")"), type_invalid);}
                    | call_expr                     {   $$ = $1; }
                    | struct_constructor            {   $$ = sq_Expression("error", $1, type_invalid); }
                    | value                         {   $$ = $1; }
                    | clone_expr                    {   $$ = sq_Expression("error", $1, type_invalid); }
                    | length_expr                   {   $$ = $1;}
                    | slice_expr                    {   $$ = sq_Expression("error", $1, type_invalid);};

call_expr       :   function_call                               {   
                                                                    $$ = $1; 
                                                                }
                        /* Casts*/
                    | primitive_type LPAREN expr RPAREN         {   
                                                                    TypeCategory typeCategory = sq_findTypeCategory(sqContext, $1);
                                                                    checkCastRule(sqContext, $1, typeCategory, $3);
                                                                    char * translationExpr =  sq_genCastExpr(sqContext, $1, typeCategory, $3);
                                                                    $$ = sq_Expression($1, translationExpr, typeCategory); 
                                                                }
                    | array_type LPAREN expr RPAREN             {   
                                                                    TypeCategory typeCategory = sq_findTypeCategory(sqContext, $1);
                                                                    checkCastRule(sqContext, $1, typeCategory, $3);
                                                                    char * translationExpr =  sq_genCastExpr(sqContext, $1, typeCategory, $3);
                                                                    $$ = sq_Expression($1, translationExpr, typeCategory); 
                                                                };

clone_expr      : CLONE LPAREN expr RPAREN                      {   const char * values[] = {"clone", "(", sq_exprToStr($3), ")"};
                                                                    $$ = concat_n(4, values); };

length_expr     :  member DOT LENGTH                            {   //member deve ser um array ou string
                                                                    char * memberLengthStr = sq_genLenghtExpr (sqContext,$1);
                                                                    if (memberLengthStr== NULL){
                                                                        sq_putError (sqContext,"Invalid member length");
                                                                    }
                                                                    char * translationExpr = memberLengthStr != NULL ? memberLengthStr : concat(sq_memberToString($1),".length");
                                                                    $$ = sq_Expression("int",translationExpr, type_integer);
                                                                };

slice_expr      : term 
                    LBRACKET opt_expr COLON opt_expr RBRACKET   {   const char * values[] = {sq_exprToStr($1), "[ ", $3, " : ", $5, " ]"};
                                                                    $$ = concat_n(6, values);};
                                                                    
opt_expr        : /*Vazio*/                         {   $$ = "";}
                    | expr                          {   $$ = sq_exprToStr($1);};
                                                                                       
lvalue_term     :  member                           { 
                                                        $$ = sq_memberToExpression(sqContext, $1);
                                                    }
                    | index_access                  { $$ = $1;}
                    | rvalue_term DOT member        { $$ = sq_Expression("error", concat3(sq_exprToStr($1), ".", sq_memberToString($3)), type_invalid);}
                    | index_access DOT member       { $$ = sq_Expression("error", concat3(sq_exprToStr($1), ".", sq_memberToString($3)), type_invalid);};

index_access    : term LBRACKET expr RBRACKET       {  /* //TODO: checar se tipo de term é array e tipo de expr é inteiro
                                                        const char * values[] = {sq_exprToStr($1), "[", sq_exprToStr($3), "]"};
                                                        char * indexAccessStr = concat_n(4, values);
                                                        $$ = sq_Expression("error", indexAccessStr, type_invalid);
                                                        */
                                                       
                                                        
                                                          char * indexAccessStr = sq_genIndexAccess(sqContext,$1, $3);
                                                           $$ = sq_Expression("error", indexAccessStr, type_invalid);
                                                        
                                                    };

value           : NUMBER_LITERAL                    {   $$ = sq_Expression("number_literal" , $1, type_integer);} 
                    | REAL_LITERAL                  {   $$ = sq_Expression("real_literal"   , $1, type_real);}
                    | STRING_LITERAL                {   $$ = sq_Expression("string_literal" , $1, type_string); }
                    | BOOLEAN_LITERAL               {   $$ = sq_Expression("boolean_literal", $1, type_boolean); }
                    | array_literal                 {   $$ = $1; }
                    | SWITCH_VALUE                  {   $$ = sq_Expression("error", "switch_value", type_invalid);};
                    
array_literal   : ARRAY_SYMBOL                      {   
                                                        char * exprStr = sq_genCreateEmptyArray();
                                                        $$ = sq_Expression(ARRAY_LITERAL_TYPE, exprStr, type_array);
                                                        free(exprStr);
                                                    }
                    | LBRACKET expr_list RBRACKET   {   
                                                        type itemType = sq_getArrayItemType(sqContext, $2);
                                                        if($2->size > 0 && itemType.category == type_invalid){
                                                            char * expListStr = joinList($2, ", ", sq_ExpressionStringConverter);
                                                            char * errMsg = concat4("Failed to create array for expressions: '", 
                                                                                        expListStr, "' of type: ", itemType.typename);
                                                            sq_putError(sqContext, errMsg);
                                                            free(expListStr);
                                                            free(errMsg);
                                                        }
                                                        else{//Garantir que tipo array existe na tabela (evitar erros)
                                                            sq_declareArrayType(sqContext, itemType.typename);
                                                        }
                                                        type arrayType = sq_getArrayType(sqContext, $2);
                                                        char * arrayExpr = sq_genArrayLiteralCreator(sqContext, itemType, $2);
                                                        $$ = sq_Expression(arrayType.typename, arrayExpr, type_array);
                                                    }
                    | NEW type 
                        LBRACKET expr RBRACKET      {   
                                                        char * createArrayStr = sq_genSizedArray(sqContext, $2, $4);
                                                        char * arrayType = concat($2, "[]");
                                                        $$ = sq_Expression(arrayType,  createArrayStr, type_array);
                                                    };

member          : ID                                {   
                                                        Category category = sq_findSymbolCategory(sqContext, $1);
                                                        $$ = sq_Member($1,$1, category, NULL);
                                                        
                                                        sq_checkExistMember(sqContext, $$);
                                                        
                                                        free($1);
                                                    }
                    | member DOT ID                 {   
                                                        char * memberTableKey = sq_makeMemberTableKey(sqContext, $3, $1);
                                                        Category category = sq_findSymbolCategory(sqContext, memberTableKey);
                                                        $$ = sq_Member($3, memberTableKey, category, $1);
                                                        
                                                        sq_checkExistMember(sqContext, $$);
                                                        
                                                        free(memberTableKey);
                                                        free($3); };

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

operator        :   //Aritméticos
                    PLUS                            { $$ = strdup("+");}
                    | MINUS                         { $$ = strdup("-");}
                    | TIMES                         { $$ = strdup("*");}
                    | DIVIDE                        { $$ = strdup("/");}
                    | MOD                           { $$ = strdup("%");}
                    //Binários
                    | BITOR                         { $$ = strdup("|");}
                    | BITAND                        { $$ = strdup("&");}
                    | SHIFTL                        { $$ = strdup("<<");}
                    | SHIFTR                        { $$ = strdup(">>");}
                    //Lógicos
                    | OU                            { $$ = strdup("||");}
                    | OR                            { $$ = strdup("or");}
                    | EE                            { $$ = strdup("&&");}
                    | AND                           { $$ = strdup("and");}
                    //Relacionais
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
			        | try_catch     {$$ = $1;};


for             : FOR {startScope("for");}
                    for_header block_body     { $$ = sq_genForBlock(sqContext, $3, $4);
                                                finishScope(); };
                                                        					
for_header      : LPAREN 
                    for_statement SEMICOLON 
                    for_expr      SEMICOLON 
                    for_statement 
                  RPAREN                        {   //const char * values[] = {"(",$2,";", $4, ";",$6 ,")"};
                                                    $$ = sq_ForHeader($2, $4, $6);};
                                        
for_statement   : /*Vazio*/                     {$$ = "";}
			        | std_statement             {$$ = $1;};

for_expr        : /*Vazio*/                     {$$ = "";}
			        | binary_expr               {$$ = sq_exprToStr($1);};

if 		        : if_block				                    {   $$ = sq_genIfBlock($1);}
    		        | if_block else_block	                {   $$ = sq_genIfElseBlock($1, $2);};

if_block 	    : IF {startScope("if");}
                    conditional_test block_body 	        {   $$ = sq_IfStruct(sq_makeScopeId(sqContext, "if_"), $3, $4);
                                                                finishScope();};

else_block      : ELSE {startScope("else");} 
                     block_body	                            {   $$ = $3;
                                                                finishScope();  }
   			        | ELSE if		                        {   $$ = $2;};

while 		    : WHILE {startScope("while");}
                    conditional_test block_body             {   $$ = sq_genWhile(sqContext, $3, $4);
                                                                finishScope();};

do_while        : DO {startScope("do");}
                    block_body 
                    WHILE conditional_test SEMICOLON        {   $$ = sq_genDoWhile(sqContext, $3, $5);
                                                                finishScope();};

try_catch 	    : TRY   { startScope("try");}
                            block_body 
                        { finishScope();}
                  CATCH { startScope("catch");} 
                            block_body
                        { finishScope();}                   {   $$ = concat4("try",$3,"catch", $7);};

conditional_test: LPAREN expr RPAREN			            {   $$ = concat3("(", sq_exprToStr($2), ")");};
%%

int main (int argc, char ** argv) {
    //CommandLineOptions * options = sq_receiveArgs(argc, argv);
    //if(options != NULL && options->output != NULL){
    //    printf("%s -o %s\n", argv[0], options->output);
    //    output_file = options->output;
    //}
    sqContext = sq_SquirrelContext();
    
    int exitCode =  yyparse ( );
    
    sq_DestroySquirrelContext(sqContext);
        
    return exitCode;
}

int yyerror (char *s) {fprintf (stderr, "%s\n", s);}