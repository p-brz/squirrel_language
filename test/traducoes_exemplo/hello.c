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
#include "sq_main.incl"
