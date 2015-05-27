#ifndef SQUIRREL_CONTEXT_H
#define SQUIRREL_CONTEXT_H

typedef struct hashtable hashtable;
typedef struct arraylist arraylist;

typedef struct{
    hashtable * symbolTable;
    arraylist * scopeList;
    int scopeIdCounter; //contador para gerar ids de escopos
} SquirrelContext;

SquirrelContext * sq_SquirrelContext();
void sq_DestroySquirrelContext(SquirrelContext * context);

#endif
