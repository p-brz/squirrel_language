#include "sqlib.h"
    
int program_main(){
    int counter = 0;
    FOR_START(){
    FOR_HEADER(for_1, int i=0, i < 10, ++i);
        counter++;
    
    FOR_END(for_1);
    }
    
    printf("%s%s\n","counter == 10? ", boolean_toCString(counter == 10));
    
    WHILE_START(while_1, counter < 12){
        printf("%s\n", "Isto deve ser executado 2 vezes!");
        ++counter;
    WHILE_END(while_1);
    }
    
    DO_START(do_1){
        printf("%s\n","Isto deve ser executado 3 vezes!");
        ++counter;
    DO_END(do_1, counter < 15);
    }
    
    free_memory();
    return 0;
}

#include "sq_main.incl"
