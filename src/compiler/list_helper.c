#include "list_helper.h"

#include "string_helpers.h"
#include <stdlib.h>
#include <stdio.h>

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



arraylist * copyList(arraylist * list, ValueDuplicator duplicator){
    arraylist * copyList = createList(NULL);
    int i;
	void* value;
	arraylist_iterate(list, i, value) {
	    void * copyValue = duplicator == NULL ? value : duplicator(value);
	    appendList(copyList, copyValue);
	}
	
	return copyList;
}
int searchItem(arraylist *list, void * expectedValue, EqualsComparator comparator){
    if(list == NULL || expectedValue == NULL || comparator == NULL){
        printf("Em %s:%d::searchItem. Erro: parâmetro NULO", __FILE__, __LINE__);
        return -2;
    }
    
    int i;
	void* value;
	arraylist_iterate(list, i, value) {
	    if(comparator(expectedValue, value)){
	        return i;
	    }
	}
	return -1;
}
bool existItem(arraylist *list, void * value, EqualsComparator comparator){
    return searchItem(list, value, comparator) >= 0;
}
bool listIsEmpty(arraylist * list){
    return arraylist_size(list) > 0 ? false : true;
}

char * joinList(arraylist * namelist, const char * symbol, StringConverter itemConverter){
    if(listIsEmpty(namelist)){
        return cpyString("");
    }
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



void destroyList(arraylist * namelist){
    arraylist_destroy(namelist);
}


bool StrEqualsComparator(void * value1, void * value2){
    if(value1 == value2){ 
        return true;
    }
    
    if(value1 != NULL && value2 != NULL){
        bool equals =  (strcmp((const char *)value1, (const char *)value2) == 0);
        return equals;
    }
    return false;
}
