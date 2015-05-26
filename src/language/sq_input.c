#include "sq_input.h"
#include "string_helpers.h"

#include <stdlib.h>
#include <stdio.h>

#define BASE_CAPACITY 100

static char * resizeLine(char * line, size_t * capacityPtr){
    size_t newCapacity = *capacityPtr * 2;
    char * tmp = realloc(line, newCapacity);
    if(tmp == NULL){//falhou
        free(line);
        return NULL;
    }
    *capacityPtr = newCapacity;
    
    return tmp;
}

//Baseado em: http://stackoverflow.com/a/314422
static char * readCStringLine(){
    size_t length = 0;
    size_t capacity = BASE_CAPACITY;
    
    char * line = calloc(capacity , sizeof(char));
    if(line == NULL){ //sem memória
        return NULL;
    }
    
    int c = getchar();
    while(c != EOF && c != '\n'){//Não inclui '\n' na string
        if(length + 1 == capacity){//deixa sempre 1 caractere sobrando para '\0'
            line = resizeLine(line, &capacity); 
            if(line == NULL){
                return NULL;
            }
        }
        
        line[length] = c;
        ++length;
        
        c = getchar();   
    }
    line[length] = '\0';
    
    char * result = cpyString(line);
    free(line);
    return result;
}

string readline(){
    string line = empty_String();
    char * lineCstr = readCStringLine();
    if(lineCstr != NULL){
        line = create_String(lineCstr);
        free(lineCstr);
    }
    return line;
}
