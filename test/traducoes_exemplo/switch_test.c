#include "sqlib.h"

boolean verifyResponse(const string response){
    SWITCH_START(switch_1){
        //Captura de expressão do switch em switch_value (o tipo é o mesmo da entrada, mas o const é obrigatório)
        const string switch_value = response; //corresponde a uma atribuição
        incrRefCount(switch_value.memory);
        
        WHEN_START(switch_1, when_1, equals_String(switch_value, create_String("S")) || equals_String(switch_value, create_String("s")))
        {
            printf("%s\n", "Que legal. Valeu!");
            //Ao encontrar um return decrementa todas as variáveis de tipos compostos
            //alocadas no escopo da função e libera memória
            decrRefCount(switch_value.memory);
            free_memory();
            return true;
        WHEN_END(switch_1, when_1);
        }
        WHEN_START(switch_1, when_2, equals_String(switch_value, create_String("N")) || equals_String(switch_value, create_String("n"))){
            printf("%s\n","Ah, que pena. Quem sabe da próxima vez.");
            
            decrRefCount(switch_value.memory);
            free_memory();
            return true;
        WHEN_END(switch_1, when_2);
        }
        DEFAULT_START(switch_1){
            printf("%s\n","Foi mal, mas eu não entendi o que você quis dizer. Eu só entendo 'S' ou 'N'.");
            printf("%s\n","Vou perguntar de novo.");
        DEFAULT_END(switch_1);
        }
    
    /*END do switch (e outras estruturas de repetição deve estar dentro do 
        escopo do bloco, para permitir decrementar variáveis)*/
    SWITCH_END(switch_1);//a contagem dos switchs (e outras estruturas de repeticao) pode (não precisa) ser local ao escopo da função
        //Decrementar variáveis alocadas no escopo
        decrRefCount(switch_value.memory);
    }
    free_memory();
    return false;
}

int program_main(){

    FOR_START(for_1){
    FOR_HEADER(for_1, int i=0, i < 10, ++i);
    
        printf("%s%s\n","i é: ", int_toCString(i));
        
        SWITCH_START(){
            WHEN_START(switch_1, when_1, i==0){
                printf("%s\n","Este bloco deve ser executado apenas quando i == 0");
            WHEN_END(switch_1, when_1);
            }
            WHEN_START(switch_1, when_2, i >=0 and i<3){
                printf("%s\n","Este bloco não deve ser executado quando i == 0 (devido ao bloco anterior).");
                printf("%s\n","porém, ele irá executar quando i for 1 ou 2.");
            WHEN_END(switch_1, when_2);
            }
            WHEN_START(switch_1, when_3, i > 5 and i < 10){
                printf("%s\n","Este bloco irá executar se i for 6,7,8 ou 9");
            WHEN_END(switch_1, when_3);
            }
            DEFAULT_START(switch_1){
                printf("%s\n","Este bloco será executado nos casos restantes. Ou seja, quando i for 3, 4 ou 5");
            DEFAULT_END(switch_1);
            }
            
        SWITCH_END(switch_1);
        }
    FOR_END(for_1);
    }
    
    boolean finishRead = false;
    DO_START(do_1){
        printf("%s\n","Você achou esse programa legal(S/N)?");
        string response = readline();
        incrRefCount(response.memory);
        
        finishRead = verifyResponse(response);
        
        //No cado do Do-While, o decremento de variáveis deve ser incluso antes do END
        decrRefCount(response.memory);
    DO_END(do_1, !finishRead);
    }
    printDebugMemory();

    printf("%s\n","tchau!");
    free_memory();
    return 0;
}

#include "sq_main.incl"
