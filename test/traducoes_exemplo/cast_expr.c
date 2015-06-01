#include "sqlib.h"

//Tabela de tipos (parcial) - deverá conter todos os tipos primitivos e os definidos no programa
//Esta tabela deve ser inclusa em todos os programas (omitindo dos exemplos por simplicidade)
const type TYPE_TABLE[] = {{"int", type_integer, NULL}, {"double", type_real, NULL}, {"boolean", type_boolean,NULL}, {"string", type_string, NULL}};

//Cada tipo terá um índice correspondente na tabela, identificado por um "define" 
//no formato: TYPE_<typename>
#define TYPE_int 0
#define TYPE_double 1
#define TYPE_boolean 2
#define TYPE_string 3


int program_main(){
    byte   b = 10;
    short  s = 100;
    int    i = 1000;
    long   l = 12345678911; 
    float  f = 0.5;
    double d = -0.100; 
    //Conversões de numeros para string:
    string bStr = integer_to_string(b);
    string sStr = integer_to_string(s);
    string iStr = integer_to_string(i);
    string lStr = integer_to_string(l);
    string fStr = real_to_string(f);
    string dStr = real_to_string(d);
    string numlit_str  = integer_to_string(-100);
    string numlit2_str = integer_to_string(1023);
    string reallit_str = real_to_string(100.01);
    //string stringlit_str = string("abc");
    string stringlit_str = create_String("abc");
    
    //Conversões de string para números
    b = string_to_byte(bStr)   + 1;
    s = string_to_short(sStr)  + 1;
    i = string_to_int(iStr)    + 1;
    l = string_to_long(lStr)   + 1;
    f = string_to_float(fStr)  + 1;
    d = string_to_double(dStr) + 1;
    
    int failedInt = string_to_int(create_String("i am a string. muahahaha"));
    float failedFloat = string_to_float(create_String("i am another string. ihihihihihi"));
    
    printf("%s%s\n","bStr: ", string_toCString(bStr));
    printf("%s%s\n","sStr: ", string_toCString(sStr));
    printf("%s%s\n","iStr: ", string_toCString(iStr));
    printf("%s%s\n","lStr: ", string_toCString(lStr));
    printf("%s%s\n","fStr: ", string_toCString(fStr));
    printf("%s%s\n","dStr: ", string_toCString(dStr));
    printf("%s%s\n","numlit_str: ", string_toCString(numlit_str));
    printf("%s%s\n","numlit2_str: ", string_toCString(numlit2_str));
    printf("%s%s\n","reallit_str: ", string_toCString(reallit_str));
    printf("%s%s\n","stringlit_str: ", string_toCString(stringlit_str));
    
    printf("%s\n", integer_toCString(b));
    printf("%s\n", integer_toCString(s));
    printf("%s\n", integer_toCString(i));
    printf("%s\n", integer_toCString(l));
    printf("%s\n", integer_toCString(f));
    printf("%s\n", integer_toCString(d));
    printf("%s\n", real_toCString(failedInt));
    printf("%s\n", real_toCString(failedFloat));
    
    //Conversão entre boolean e string
    boolean bl = false;
    string blStr = boolean_to_string(bl);
    string booleanlit_str = boolean_to_string(true);
    bl = string_to_boolean(booleanlit_str);
    
    printf("%s%s\n", "blStr:", string_toCString(blStr));
    printf("%s%s\n","booleanlit_str:", string_toCString(booleanlit_str));
    printf("%s%s\n","boolean(booleanlit_str):", boolean_toCString(bl));
    
    //Conversão de type para string
    //Não deve ser possível converter de string para typename
    //string typeStr = string(typeof(int));
    string typeStr = typename(TYPE_TABLE[TYPE_int]);
    //type t = typeof(true);
    type t = TYPE_TABLE[TYPE_boolean];
    string typeStr2 = typename(t);
    
    printf("%s%s\n","typeStr1: ", string_toCString(typeStr));
    printf("%s%s\n","typeStr2: ", string_toCString(typeStr2));
    printf("%s%s\n","t: ", typename(t).cstr);
    
    free_memory();
    return 0;
}

#include "sq_main.incl"