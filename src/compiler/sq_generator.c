#include "sq_generator.h"
#include "string_helpers.h"

const char * INCLUDES = "#include \"sqlib.h\"\n\n";

const char * MAIN = 
    "int main(){\n"
    "    squirrel_setup();\n"
    "    int retValue = program_main();\n"
    "    free_All();\n"
    "    return retValue;\n"
    "}\n";

char * gen_program(const char * declarationsList){
    return concat3(INCLUDES, declarationsList, MAIN);
}
