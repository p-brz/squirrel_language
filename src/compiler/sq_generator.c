#include "sq_generator.h"
#include "string_helpers.h"

const char * INCLUDES = "#include \"sqlib.h\"\n\n";

const char * MAIN_INCLUDE = "\n#include \"sq_main.incl\"\n";

char * gen_program(const char * declarationsList){
    return concat3(INCLUDES, declarationsList, MAIN_INCLUDE);
}
