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
#define TYPE_short 2
#define TYPE_real_literal 3
#define TYPE_void 4
#define TYPE_number_literal 5
#define TYPE_float 6
#define TYPE_double 7
#define TYPE_int 8
#define TYPE_long 9
#define TYPE_byte 10
#define TYPE_string 11
#define TYPE_object 12

int program_main(){
        int a = 10;
        string str;
        printf("%s\n","Informe um inteiro");
        a = string_to_int(readline());
        print(a);
        return 0;
}

#include "sq_main.incl"