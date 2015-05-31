#include "sqlib.h"

int program_main(){
    byte   b = 10;
    short  s = 100;
    int    i = 1000;
    long   l = 12345678911; 
    float  f = 0.5;
    double d = -0.100; 
    //Conversões de numeros para string:
    string bStr = cast_integer_to_string(b);
    string sStr = cast_integer_to_string(s);
    string iStr = cast_integer_to_string(i);
    string lStr = cast_integer_to_string(l);
    string fStr = cast_real_to_string(f);
    string dStr = cast_real_to_string(d);
    string numlit_str  = cast_integer_to_string(-100);
    string reallit_str = cast_real_to_string(100.01);
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
    
    return 0;
}

#include "sq_main.incl"