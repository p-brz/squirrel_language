#include "sqlib.h"

#include "string_helpers.h"

typedef struct type{
    const char * typename;
    struct type * arrayOf;//Tipos array terão um apontador para o tipo gerador
} type;

type create_Type(const char * typename, type * arrayOf){
    type t;
    t.typename = typename;
    t.arrayOf = arrayOf;
    return t;
}

string typename(const type t){
    return create_String(t.typename);
}

//boolean caststo(type t1, type t2){
//    
//}
//------------------------------------------------------------------------------
//Tabela de tipos (parcial) - deverá conter todos os tipos primitivos e os definidos no programa
type TYPE_TABLE[] = {{"int", NULL}, {"double", NULL}, {"Moeda", NULL}};

//Cada tipo terá um índice correspondente na tabela, identificado por um "define" 
//no formato: TYPE_<typename>
#define TYPE_int 0
#define TYPE_double 1
#define TYPE_Moeda 2 //O typeof pode ser definido antes do tipo em si ser encontrado

typedef enum {Moeda_Cara, Moeda_Coroa} Moeda;
//Conversor de enum para String
const char * Moeda_toCString(Moeda value){
    if(value == Moeda_Cara)
        return "Cara";
    if(value == Moeda_Coroa)
        return "Coroa";
    return "";
}

int program_main(){
    //'typeof' será convertido para uma consulta na tabela pelo tipo apropriado
    type t1 = TYPE_TABLE[TYPE_int];
    type t2 = TYPE_TABLE[TYPE_double]; //Tipo padrão para literal real é double
    //Outra alternativa para literais (incluir o tipo como distinto??)
    //type t2 = TYPE_TABLE[TYPE_real_literal];

    //typename retorna uma string e a regra do print "converte" para uma c-string (acessando o membro 'cstr' de string)    
    printf("%s%s\n", "t1: ", typename(t1).cstr);
    printf("%s%s\n", "t2: ", typename(t2).cstr);
    printf("%s%s\n", "enum Moeda: ", typename(TYPE_TABLE[TYPE_Moeda]).cstr);
/*
    print("t1 casts to t2 ? ", t1 caststo(t2) );
    print("enum Moeda casts to string? ", Moeda caststo(string) );
    print("boolean caststo ( 1 caststo(double) )? ", boolean caststo( 1 caststo(double) ) );
*/

    return 0;
}

#include "sq_main.incl"
