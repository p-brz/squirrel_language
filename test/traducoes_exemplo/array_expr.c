#include <sqlib.h>

typedef struct SomeStruct{
    //int[] numbers;
    Array numbers; 
} SomeStruct;
SomeStruct * construct_SomeStruct(Array numbers){
    SomeStruct * structValue = (SomeStruct *)malloc(sizeof(SomeStruct));
    structValue->numbers = numbers;
    return structValue;
}

//int[ ] arrayFunction(){
Array arrayFunction(){
    return create_Array(sizeof(int), 2, (int[]){10,11});
}

//int[][] matrixFunction(){
Array matrixFunction(){
    //return [[1,2,3],[4,5,6],[7,8,9]];
    return create_Array(sizeof(Array), 3,
                        (Array[]){
                            create_Array(sizeof(int), 3, (int[]){1,2,3}),
                            create_Array(sizeof(int), 3, (int[]){4,5,6}),
                            create_Array(sizeof(int), 3, (int[]){7,8,9})}
                    );
}

//int main(string[] args){
//int program_main(Array args){
int program_main(){
    
    //int v1 = arrayFunction()[0];
    int v1 = getArrayItem(int, arrayFunction(), 0);
    //int v2 = matrixFunction()[0][1];
    int v2 = getArrayItem(int, getArrayItem(Array, matrixFunction(), 0), 1);
    
    //SomeStruct someStruct1 = SomeStruct{numbers : arrayFunction()};
    
    //int v3 = someStruct1.numbers[2];
    //int v4 = SomeStruct{numbers : arrayFunction()}[1];
    
    printf("%s%s\n","v1: ", long_toCString(v1));
    printf("%s%s\n", "v2: ", long_toCString(v2));
    //print("v3: ", v3);
    //print("v4: ", v4);
    
    return 0;
}

#include "sq_main.incl"