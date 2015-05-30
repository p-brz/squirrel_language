#include "sqlib.h"

//Tipos de função são gerados utilizando ponteiros para função
//typedef <type> (* <ID>)(<func_params>);
typedef void (* PrintFunction)();

typedef struct StructWithFunction{
    PrintFunction f;
} StructWithFunction;

//OBS.: para cada struct será gerado um construtor que recebe como parâmetro 
//todos os membros da struct
StructWithFunction * construct_StructWithFunction(PrintFunction f){
    StructWithFunction * structValue = (StructWithFunction *)malloc(sizeof(StructWithFunction));
    structValue->f = f;
    return structValue;
}

//Para structs serão geradas também funções para incrementar e decrementar o 
//refCount da struct e de todos os membros de tipos compostos (possuem um MemoryNode)

void incrRefCount_StructWithFunction(void * value){
    if(value == NULL){
        return;
    }
    StructVar * structVar = incrRefCount_StructVar(value);
    //OBS.: caso StructWithFunction tivesse membros com memória gerenciada(tipos compostos)
    //aqui seria gerado código para incrementar a referência para estes membros
}
void decrRefCount_StructWithFunction(void * value){
    if(value == NULL){
        return;
    }
    StructVar * structVar = decrRefCount_StructVar(value);
    //OBS.: caso StructWithFunction tivesse membros com memória gerenciada(tipos compostos)
    //aqui seria gerado código para decrementar a referência para estes membros
}

void printA(){
    printf("%s\n", create_String("A").cstr);
    free_memory();
}
void printB(){
    printf("%s\n", create_String("B").cstr);
    free_memory();
}
void printC(){
    printf("%s\n", create_String("C").cstr);
    free_memory();
}
void printD(){
    printf("%s\n", create_String("D").cstr);
    free_memory();
}
void printE(){
    printf("%s\n", create_String("E").cstr);
    free_memory();
}

PrintFunction functionFactory(){
    return printE;
}
    
int program_main(){
    Array functionArray = create_Array(sizeof(PrintFunction), 3, (PrintFunction[]){printA,printB,printC});
    incrRefCount_Array(functionArray, NULL);
    
    StructVar printable = create_Struct(construct_StructWithFunction(printD));
    incrRefCount_StructWithFunction(&printable);

    (getArrayItem(PrintFunction, functionArray, 2))();
    getArrayItem(PrintFunction, functionArray, 0)();
    getArrayItem(PrintFunction, functionArray, 1)();
    getArrayItem(PrintFunction, functionArray, 2)();
    getStructMember(StructWithFunction, printable,f)();
    functionFactory()();
    
    //Decrementar variáveis alocadas no escopo
    decrRefCount_Array(functionArray, NULL);
    decrRefCount_StructWithFunction(&printable);
    free_memory();
    
    //Imprimindo memória para verificar que tudo foi desalocado
    printDebugMemory();
    return 0;
}

#include "sq_main.incl"
