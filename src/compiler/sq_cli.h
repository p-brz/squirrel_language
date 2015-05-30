#ifndef SQ_CLI_H
#define SQ_CLI_H

/******************************************************************************
 * Funções e tipos relativos à interface de linha de comando
 * */

#include "squirrel_context.h"

typedef struct{
    char * output;
} CommandLineOptions;

CommandLineOptions * sq_receiveArgs(int argc, char ** argv);

void sq_printStatus(SquirrelContext * sqContext);
void sq_writeFile(const char * programContent, const char * outputFile);

#endif