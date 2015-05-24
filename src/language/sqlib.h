#ifndef SQLIB_H
#define SQLIB_H

#include "sq_types.h"
#include "sq_memory.h"
#include "sq_control.h"
#include <stdio.h> //printf

void squirrel_setup(){
    sq_memory_setup();
}

#endif
