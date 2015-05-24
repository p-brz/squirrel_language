#include "sqlib.h"

int main(){
    squirrel_setup();
    //Obs.: literais (númericos) podem ser convertidos diretamente para string
    //isso vai depender se o tipo do literal for diferenciado do tipo da variável
    printf("%s%s\n","Number: " , "10");
    printf("%s%s\n","Boolean: ", boolean_toCString(true)); //ver sq_types.h para definição de literais de boolean
    printf("%s%s\n","Real: "   , "0.1");
    
    boolean bValue = true;
    //Obs.: números literais poderão receber um terminador para indicar que são
    //do tipo apropriado durante a tradução
    long longNumber = 12345l;
    float floatNumber = 0.5f;
    
    //Variáveis não podem ser convertidas diretamente para cstring
    printf("%s%s\n","Boolean var: ", boolean_toCString(bValue));
    printf("%s%s\n","Long var: "   , long_toCString(longNumber));
    printf("%s%s\n","float var: "  , float_toCString(floatNumber));
    
    free_memory();
    printDebugMemory();
    return 0;
}
