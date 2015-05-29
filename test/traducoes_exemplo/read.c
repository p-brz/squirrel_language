#include "sqlib.h"

int program_main ()
{
    string minha;
    
    minha = read(10);
    
    printf( "%s\n", minha.cstr );
    decrRefCount(minha.memory);
    
    free_memory();
    return 0;
}

#include "sq_main.incl"