#include "sqlib.h"

const type TYPE_TABLE[] = {{ "boolean", type_boolean, NULL },
        { "type", type_type, NULL },
        { "short", type_integer, NULL },
        { "real_literal", type_real, NULL },
        { "void", type_void, NULL },
        { "number_literal", type_integer, NULL },
        { "float", type_real, NULL },
        { "double", type_real, NULL },
        { "int", type_integer, NULL },
        { "long", type_integer, NULL },
        { "byte", type_integer, NULL },
        { "string", type_string, NULL },
        { "object", type_object, NULL }};

#define TYPE_boolean 0
#define TYPE_type 1
#define TYPE_int_ARRAY 2
#define TYPE_short 3
#define TYPE_real_literal 4
#define TYPE_void 5
#define TYPE_number_literal 6
#define TYPE_float 7
#define TYPE_double 8
#define TYPE_int 9
#define TYPE_long 10
#define TYPE_byte 11
#define TYPE_string 12
#define TYPE_object 13

boolean isOnArray(Array array, int value){
    int i = 0;
    FOR_START() {
        FOR_HEADER(for_3, i = 0, i < array.length, i++);
        IF_START(if_3, (getArrayItem(int, array, i) == value)) {
            return true;
        }
        IF_END(if_3);

        FOR_END(for_3);
        //TODO: Gerar decremento de variáveis (de tipos compostos) do escopo atual 
    }

    return false;
}


int program_main(){
        Array A1 = create_Array(sizeof(int), 20, NULL);
        printf("%s\n","Informe n:");
        int n = string_to_int(readline());
        FOR_START(){
        FOR_HEADER(for_5, int i = 0, i < n, ++i);
                printf("%s%s\n","informe elemento ",long_toCString(i));
                getArrayItem(int, A1, i) = string_to_int(readline());
        FOR_END(for_5);
        //TODO: Gerar decremento de variáveis (de tipos compostos) do escopo atual 
        }

        printf("%s\n","Informe k:");
        int k = string_to_int(readline());
        boolean b = isOnArray(A1, k);
        printf("%s%s%s%s\n"," o valor",long_toCString(k)," esta no array?",boolean_toCString(b));
        return 0;
}

#include "sq_main.incl"