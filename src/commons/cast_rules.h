#ifndef CAST_RULES_H
#define CAST_RULES_H

#include <stdbool.h>
#include "sq_metatype.h"

/**Para cada tipo numérico, retorna um inteiro indicando sua "largura". 
    Tipos com "largura" menor podem ser convertidos em tipos com "largura" maior.
*/
int getTypeNumberWidth(const char * typename);

bool isNumber(TypeCategory category);

/** checa se um tipo com nome typename1 e categoria 'category1' pode ser convertido
implicitamente (coerção) para um outro tipo de nome 'typename2' e categoria 'category2'.
OBS.: não checa se tipos são iguais*/
bool can_coerce_to(const char * typename1, TypeCategory category1
                        ,const char * typename2, TypeCategory category2);

/** checa se um tipo de categoria 'category' pode ser convertido através de
cast para string.
OBS.: não checa se tipo pode sofrer coerção ou se é igual à string*/
bool can_force_cast_to_string(TypeCategory category);

/** checa se um tipo de categoria 'category1' pode ser convertido para um outro 
de categoria 'category2' através de cast.
OBS.: não checa se tipo pode sofrer coerção ou se é igual à string*/
bool can_force_coerce_to(TypeCategory cat1, TypeCategory cat2);

#endif
