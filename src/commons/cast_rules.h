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
BOOL can_coerce_to(const char * typename1, TypeCategory category1
                        ,const char * typename2, TypeCategory category2);

BOOL can_force_cast_to_string(TypeCategory category);

BOOL can_force_coerce_to(TypeCategory cat1, TypeCategory cat2);

#endif
