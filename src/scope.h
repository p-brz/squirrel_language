#ifndef SCOPE_H
#define SCOPE_H

#include "squirrel_context.h"

void sq_startScope(SquirrelContext * sqContext, const char * scopeName);
void sq_finishScope(SquirrelContext * sqContext);

#endif
