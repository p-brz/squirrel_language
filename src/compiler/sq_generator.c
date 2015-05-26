#include "sq_generator.h"
#include "string_helpers.h"

#include "symboltable_types.h"
#include "hashtable.h"
#include "list_helper.h"

#include <stdlib.h> //NULL

const char * INCLUDES = "#include \"sqlib.h\"\n\n";

const char * MAIN_INCLUDE = "\n#include \"sq_main.incl\"\n";

const char * DEFINE_TYPE_BEGIN = "#define TYPE_";
const char * TYPE_TABLE_DECLARATION = "const type TYPE_TABLE[] = ";

/***************************** FUNÇÕES AUXILIARES *****************************/

char * generateTypeTable(SquirrelContext * ctx);

char * gen_TypeIDDefine(TableRow * tableRow, int typeNumber);
char * gen_TypeTableItem(TableRow * tableRow);
char * getTypeCategoryStr(TableRow * tableRow);
/******************************************************************************/

char * gen_program(SquirrelContext * ctx, const char * declarationsList){
    return concat4(INCLUDES, generateTypeTable(ctx), declarationsList, MAIN_INCLUDE);
}

char * generateTypeTable(SquirrelContext * ctx){
    hashtable * symbolTable = ctx->symbolTable;
    
    arraylist * typeIDList = createList(NULL);
    arraylist * typeTableList = createList(NULL);
    
    if(symbolTable != NULL){
        int typeCount = 0;
        int i;
        for(i=0; i < symbolTable->capacity; ++i){
            char * key = symbolTable->body[i].key;
            if(key != NULL){
                TableRow * tableRow = (TableRow *)symbolTable->body[i].value;
                if(isType(tableRow)){
                    char * typeID_define = gen_TypeIDDefine(tableRow, typeCount);
                    ++typeCount;
                    appendList(typeIDList, typeID_define);
                    
                    char * typeTableItem = gen_TypeTableItem(tableRow);
                    appendList(typeTableList, typeTableItem);
                }
            }
        } 
    }
    
    char * typeTableItems = joinList(typeTableList, ",\n\t", NULL);
    char * typeTableStr = concat4(TYPE_TABLE_DECLARATION, "{", typeTableItems,"};\n\n");
    free(typeTableItems);
    destroyList(typeTableList);
    
    char * definesStr =  joinList(typeIDList, "\n", NULL);
    destroyList(typeIDList);
    
    char * result = concat3(typeTableStr, definesStr, "\n\n");
    free(typeTableStr);
    free(definesStr);
    
    return result;
}


char * gen_TypeIDDefine(TableRow * tableRow, int typeNumber){
    char * numberStr = intToString(typeNumber);
    char * result = concat4(DEFINE_TYPE_BEGIN, tableRow->name, " ", numberStr);
    free(numberStr);
    return result;
}

char * gen_TypeTableItem(TableRow * tableRow){
    //{"int", type_integer, NULL}
    const char * typeName = tableRow->name;
    char * typeCategoryStr = getTypeCategoryStr(tableRow);
    
    const char * values[] = { "{ \"", typeName, "\", ", typeCategoryStr, ", NULL }" };
    char * tableItemStr = concat_n(5, values);
    
    return tableItemStr;
}

char * getTypeCategoryStr(TableRow * tableRow){
    switch(tableRow->category){
        case categ_primitiveType:
            return cpyString(sq_typeCategoryCString(tableRow->value.primitiveValue.typeCategory));
        case categ_structType: 
            return cpyString("type_struct");
        case categ_functionType: 
            return cpyString("type_function");
        case categ_enumType:
            return cpyString("type_enum");
    }
    return cpyString("");
}