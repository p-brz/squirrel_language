#ifndef SCOPE_H
#define SCOPE_H

#include "squirrel_context.h"

void sq_startScope(SquirrelContext * sqContext, const char * scopeName);
void sq_finishScope(SquirrelContext * sqContext);

char * sq_fullScopeName(SquirrelContext * sqContext);
char * sq_makeFullScopeName(arraylist * scopeList);

char * sq_makeScopeId(SquirrelContext * sqContext, const char * prefix);
#endif
