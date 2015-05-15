#include "list_helper.h"

#include "string_helpers.h"
#include <stdlib.h>

NameList * createNameList(char * name){
    return createList(name);
}
NameList * appendNameList(NameList * namelist, char * name){
    return appendList(namelist, name);
}

arraylist * createList(void * value){
    if(value == NULL){
        return arraylist_create();
    }
    return appendList(arraylist_create(), value);
}
arraylist * appendList(arraylist * list, void * value){
    arraylist_add(list, value);
    return list;
}

char * joinList(NameList * namelist, const char * symbol, StringConverter itemConverter){
    char * result = NULL;
    int i;
	void* value;
	arraylist_iterate(namelist, i, value) {
	    char * strItem = itemConverter == NULL ? cpyString((char *)value) : itemConverter(value);
	    if(result == NULL){
	        result = strItem;
	    }
	    else{
	        char * cat = concat3(result, symbol, strItem);
	        free(result);
	        result = cat;
	    }
	}
	return result;
}

void destroyList(NameList * namelist){
    arraylist_destroy(namelist);
}

