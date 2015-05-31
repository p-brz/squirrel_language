#include <sqlib.h>

//void printArray(int[] array){
void printArray(Array array){
}

typedef enum {Dado_L1, Dado_L2, Dado_L3, Dado_L4, Dado_L5, Dado_L6} Dado;

//Cada enum deve gerar uma função para obter a string correspondente
const char * Dado_toCString(Dado value){
    if(value == Dado_L1)
        return "L1";
    if(value == Dado_L2)
        return "L2";
    if(value == Dado_L3)
        return "L3";
    if(value == Dado_L4)
        return "L4";
    if(value == Dado_L5)
        return "L5";
    if(value == Dado_L6)
        return "L6";
    return "";
}

int program_main(){
    //int[] array1 = [10, 11, 12];
    Array array1 = create_Array(sizeof(int), 3, (int []){10, 11, 12});
    Array array2 = create_Array(sizeof(int), 1, (int []){10});
    //int[] array3 = new int[10];
    Array array3 = create_Array(sizeof(int), 10, NULL);
    
    //Dado[] lancamentos = new Dado[4];
    Array lancamentos = create_Array(sizeof(Dado), 4, NULL);
    
    printf("%s%s\n","tamanho array1:", long_toCString(array1.length));
    printf("%s%s\n","tamanho array2:", long_toCString(array2.length));
    printf("%s%s\n","tamanho array3:", long_toCString(array3.length));
    printf("%s%s\n","tamanho lancamentos: ", long_toCString(lancamentos.length));
    
    return 0;
}

#include "sq_main.incl"