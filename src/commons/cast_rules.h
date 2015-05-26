#ifndef CAST_RULES_H
#define CAST_RULES_H

#ifndef BOOL
#define BOOL int
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 9
#endif

typedef enum {type_type, type_integer, type_real, type_boolean, type_string, type_object, type_enum, type_array, type_function, type_struct} TypeCategory;

/**Para cada tipo numérico, retorna um inteiro indicando sua "largura". 
    Tipos com "largura" menor podem ser convertidos em tipos com "largura" maior.
*/
int getTypeNumberWidth(const char * typename);

BOOL isNumber(TypeCategory category);

/** checa se um tipo com nome typename1 e categoria 'category1' pode ser convertido
implicitamente (coerção) para um outro tipo de nome 'typename2' e categoria 'category2'.
OBS.: não checa se tipos são iguais*/
BOOL can_coerce_to(const char * typename1, TypeCategory category1
                        ,const char * typename2, TypeCategory category2);

/** checa se um tipo de categoria 'category' pode ser convertido através de
cast para string.
OBS.: não checa se tipo pode sofrer coerção ou se é igual à string*/
BOOL can_force_cast_to_string(TypeCategory category);

/** checa se um tipo de categoria 'category1' pode ser convertido para um outro 
de categoria 'category2' através de cast.
OBS.: não checa se tipo pode sofrer coerção ou se é igual à string*/
BOOL can_force_coerce_to(TypeCategory cat1, TypeCategory cat2);

#endif
