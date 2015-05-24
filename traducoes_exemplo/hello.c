#include "sqlib.h"

int main ()
{
    //Setup deve ser incluso no main de todos os programas
    squirrel_setup();
    //print inclui '\n' no final
    printf("%s\n", "Hello World");//String literal pode ser convertida diretamente no print
    //Ao fim de cada função, a memória deverá ser liberada
    free_memory();
	return 0;
}
