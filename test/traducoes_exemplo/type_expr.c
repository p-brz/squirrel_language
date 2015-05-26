#include "sqlib.h"

//Tabela de tipos (parcial) - deverá conter todos os tipos primitivos e os definidos no programa
//Esta tabela deve ser inclusa em todos os programas (omitindo dos exemplos por simplicidade)
const type TYPE_TABLE[] = {{"int", type_integer, NULL}, {"double", type_real, NULL}, {"boolean", type_boolean,NULL}, {"string", type_string, NULL}, {"Moeda", type_enum, NULL}};

//Cada tipo terá um índice correspondente na tabela, identificado por um "define" 
//no formato: TYPE_<typename>
#define TYPE_int 0
#define TYPE_double 1
#define TYPE_boolean 2
#define TYPE_string 3
#define TYPE_Moeda 4 //O typeof pode ser definido antes do tipo em si ser encontrado

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

    printf("%s%s\n","t1 casts to t2 ? ", boolean_toCString(caststo(t1 , t2)) );
    printf("%s%s\n","enum Moeda casts to string? "
                , boolean_toCString( caststo(TYPE_TABLE[TYPE_Moeda], TYPE_TABLE[TYPE_string])) );
    printf("%s%s\n","boolean caststo ( 1 caststo(double) )? " //O tipo de caststo já é conhecido em tempo de compilação
                , boolean_toCString( caststo(TYPE_TABLE[TYPE_boolean], TYPE_TABLE[TYPE_boolean])) );


    return 0;
}

#include "sq_main.incl"
