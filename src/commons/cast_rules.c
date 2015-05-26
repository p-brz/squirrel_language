#include "cast_rules.h"


/**Para cada tipo numérico, retorna um inteiro indicando sua "largura". 
    Tipos com "largura" menor podem ser convertidos em tipos com "largura" maior.
*/
int getTypeNumberWidth(const char * typename){
    if(strEquals(typename, "number_literal"))
        return 1;
    if(strEquals(typename, "byte"))
        return 1;
    if(strEquals(typename, "short"))
        return 2;
    if(strEquals(typename, "int"))
        return 3;
    if(strEquals(typename, "long"))
        return 4;
    if(strEquals(typename, "real_literal"))
        return 5;
    if(strEquals(typename, "float"))
        return 5;
    if(strEquals(typename, "double"))
        return 6;
    return 0;
}

BOOL isNumber(TypeCategory category){
    return category == type_integer || category == type_real;
}
BOOL can_coerce_to(const char * typename1, TypeCategory category1
                        ,const char * typename2, TypeCategory category2){
    if(category2 == type_object){
        return TRUE;
    }
    else if(isNumber(category1) && isNumber(category2)){
        return getTypeNumberWidth(typename2) >= getTypeNumberWidth(typename1);
    }
    return FALSE;
}

BOOL can_force_cast_to_string(TypeCategory category){
    return isNumber(category)
            || category == type_boolean
            || category == type_object //Objeto pode ter cast forçado a qualquer tipo
            || category == type_enum   //valores de enum podem ser convertidos para string
            || category == type_type;  //conversão de type para string é feita via typename 
}

BOOL can_force_coerce_to(TypeCategory cat1, TypeCategory cat2){
    if(cat1 == type_object){ //object pode ser forçado para qualquer tipo
        return TRUE;
    }
    else if(isNumber(cat1) && isNumber(cat2)){
        return TRUE;
    }
    else if(cat2 == type_string){
        return can_force_cast_to_string(cat1);
    }
    return FALSE;
}
