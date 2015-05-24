#include "sqlib.h"

int program_main(){
    
    IF_START(if_1, true){
        printf("%s\n", "if sozinho");
    }
    IF_END(if_1);
    
    IF_START(if_2, true){
        printf("%s\n","sempre execute isto.");
    }
    ELSE_START(if_2){
        printf("%s\n","Else inútil");
    }
    ELSE_END(if_2);
    
    //Blocos utilizados para isolar o escopo do for
    {
        FOR_START(for_1, int i=0, i < 4, ++i){
            printf("%s%s\n", "i: ", int_toCString(i));
        
            IF_START(if_3, i==0){
                printf("%s\n", "\ti é zero.");
            }
            ELSE_START(if_3){
                //transforma um "else if" em um else { if ... }
                IF_START(if_4,  i==1){
                    printf("%s\n", "\ti é 1");
                }
                ELSE_START(if_4){
                    printf("%s\n", "\ti é 2 ou 3");
                    IF_START(if_5, i > 2){
                        printf("%s\n", "\t\ti é 3");
                    }
                    ELSE_START(if_5){
                        printf("%s\n", "\t\ti é 2");
                    }
                    ELSE_END(if_5);
                }
                ELSE_END(if_4);
                
            }
            ELSE_END(if_3);
            
        } 
        FOR_END(for_1);
    }
    
    return 0;
}

#include "sq_main.incl"
