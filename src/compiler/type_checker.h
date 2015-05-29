#ifndef TYPE_CHECKER_H
#define TYPE_CHECKER_H

#include <stdbool.h>
#include "squirrel_context.h"
#include "cast_rules.h"

bool checkCategoryEquals(SquirrelContext * sqContext, TypeCategory current, TypeCategory expected);

#endif