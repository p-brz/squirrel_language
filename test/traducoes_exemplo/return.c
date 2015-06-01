#include "sqlib.h"
    
string strTable(int index){
    FUNCTION_START(string);
    
    string dummyVar = create_String("dummy");
    incrRefCount(dummyVar.memory);
    
    IF_START(if_1, index == 0){
        //return "abc";
        return_ = create_String("abc");
        RETURN();
    }
    ELSE_START(if_1){
        IF_START(if_2, index == 1){
            //return "def";
            return_ = create_String("def");
            RETURN();
        }
        ELSE_START(if_2){
            IF_START(if_3, index == 2){
                string value = create_String("ghi");
                incrRefCount(value.memory);
                //return value;
                return_ = value;
                decrRefCount(value.memory);//decremento de variáveis no escopo
                RETURN();
            }
            ELSE_START(if_3){
                //return "other";
                return_ = create_String("other");
                RETURN();
            }
            ELSE_END(if_3);
        }
        ELSE_END(if_2);
        
    }
    ELSE_END(if_1);
    
    FUNCTION_END();
    //incrementa memória de variável de retorno(para não ser desalocada)
    incrRefCount(return_.memory);
    //decremento de outras variáveis no escopo
    decrRefCount(dummyVar.memory);
    //liberar memória deste escopo    
    free_memory();
    FUNCTION_RETURN();
}

void printCount(){
    //Não há FUNCTION_START pois função é void
    FOR_START(){
    FOR_HEADER(for_1, int i=0, i < 5, ++i);
        printf("%s\n",integer_toCString(i));
        IF_START(if_1, i==4){
            RETURN();
        }
        IF_END(if_1);
    FOR_END(for_1);
    }
    
    FUNCTION_END();
      
    free_memory();
    //Não há retorno pois função é void
}
int program_main(){
    FUNCTION_START(int);
    FOR_START(){
    FOR_HEADER(for_1, int i=0, i < 4, ++i);
        printf("%s\n",string_toCString(strTable(i)));
    FOR_END(for_1);
    }
    printCount();
    
    return_ = 0;
    RETURN();
    FUNCTION_END();
    free_memory();
    FUNCTION_RETURN();
}

#include "sq_main.incl"