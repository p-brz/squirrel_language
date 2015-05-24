#include "sqlib.h"

typedef enum {Naipes_Ouros, Naipes_Espadas, Naipes_Paus, Naipes_Copas} Naipes;

//Cada enum deve gerar uma função para obter a string correspondente
const char * Naipes_toCString(Naipes value){
    if(value == Naipes_Ouros)
        return "Ouros";
    if(value == Naipes_Espadas)
        return "Espadas";
    if(value == Naipes_Paus)
        return "Paus";
    if(value == Naipes_Copas)
        return "Copas";
    return "";
}

int program_main(){
    //Obs.: literais (númericos e string) podem ser convertidos diretamente para cstring
    //isso vai depender se o tipo do literal for diferenciado do tipo da variável
    printf("%s%s\n","Number: " , "10");
    printf("%s%s\n","Boolean: ", boolean_toCString(true)); //ver sq_types.h para definição de literais de boolean
    printf("%s%s\n","Real: "   , "0.1");
    printf("%s%s\n","String literal: ", "literal");
    printf("%s%s\n","Enum literal: ", Naipes_toCString(Naipes_Espadas));
    printf("\n");//print vazio
    
    boolean bValue = false;
    //Obs.: números literais poderão receber um terminador para indicar que são
    //do tipo apropriado durante a tradução
    long longNumber = 12345l;
    float floatNumber = 0.5f;
    string stringVar = create_String("hello");
    incrRefCount(stringVar.memory);
    Naipes naipesVar = Naipes_Ouros;
    
    //Variáveis não podem ser convertidas diretamente para cstring
    printf("%s%s\n","Boolean var: ", boolean_toCString(bValue));
    printf("%s%s\n","Long var: "   , long_toCString(longNumber));
    printf("%s%s\n","float var: "  , float_toCString(floatNumber));
    printf("%s%s\n","string var: ", string_toCString(stringVar));
    printf("%s%s\n","enum var: ", Naipes_toCString(naipesVar));
    
    //Variáveis de tipos compostos devem ter seu refCount decrementado ao fim do escopo
    decrRefCount(stringVar.memory);
    free_memory();
    printDebugMemory();
    return 0;
}

#include "sq_main.incl"
