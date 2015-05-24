#include "sqlib.h"

//Main do programa é convertido em "program_main"
int program_main ()
{
    //print inclui '\n' no final
    printf("%s\n", "Hello World");//String literal pode ser convertida diretamente no print
    //Ao fim de cada função(ou antes de um return), a memória deverá ser liberada
    free_memory();
	return 0;
}

//Main real deve ser embutido ao fim do programa
int main(){
    //Setup deve ser incluso no main de todos os programas
    squirrel_setup();
    //Chamada do main do programa
    int retValue = program_main();
    //Libera qualquer memória que não tenha sido desalocada
    free_All();
    return retValue;
}
