#include "sqlib.h"

#define SWITCH_EMPTY()\
    ;

#define SWITCH_END(switch_id)\
    switch_exit_##switch_id : \
        ;
            
#define WHEN_START(switch_id, when_id, condition)  \
    if(!(condition))                    \
        goto when_exit_##when_id ;
    
    
#define WHEN_END(switch_id, when_id)\
    goto switch_exit_##switch_id ;   \
    when_exit_##when_id:            \
        ;
        
#define DEFAULT_START(switch_id)    \
    ;
    
#define DEFAULT_END(switch_id)\
    goto switch_exit_##switch_id ;

/**

boolean verifyResponse(const string response){
    switch(response){
        when(switch_value == "S" || switch_value == "s"){
            print("Que legal. Valeu!");
            return true;
        }
        when(switch_value == "N" || switch_value == "n"){
            print("Ah, que pena. Quem sabe da próxima vez.");
            return true;
        }
        default{
            print("Foi mal, mas eu não entendi o que você quis dizer. Eu só entendo 'S' ou 'N'.");
            print("Vou perguntar de novo.");
        }
    }
    return false;
}
*/

int program_main(){
    //Bloco do FOR
    {
        FOR_START(for_1, int i=0, i < 10, ++i){
            printf("%s%s\n","i é: ", int_toCString(i));
            
            SWITCH_START(){
                WHEN_START(switch_1, when_1, i==0){
                    printf("%s\n","Este bloco deve ser executado apenas quando i == 0");
                }
                WHEN_END(switch_1, when_1);
                WHEN_START(switch_1, when_2, i >=0 and i<3){
                    printf("%s\n","Este bloco não deve ser executado quando i == 0 (devido ao bloco anterior).");
                    printf("%s\n","porém, ele irá executar quando i for 1 ou 2.");
                }
                WHEN_END(switch_1, when_2);
                WHEN_START(switch_1, when_3, i > 5 and i < 10){
                    printf("%s\n","Este bloco irá executar se i for 6,7,8 ou 9");
                }
                WHEN_END(switch_1, when_3);
                DEFAULT_START(switch_1){
                    printf("%s\n","Este bloco será executado nos casos restantes. Ou seja, quando i for 3, 4 ou 5");
                }
                DEFAULT_END(switch_1);
            }
            SWITCH_END(switch_1);
        }
        FOR_END(for_1);
    }
    
    /**
    
    boolean finishRead = false;
    do{
        print("Você achou esse programa legal(S/N)?");
        string response = readline();
        
        finishRead = verifyResponse(response);
    }while(!finishRead);
    */

    printf("%s\n","tchau!");
    
    return 0;
}

#include "sq_main.incl"
