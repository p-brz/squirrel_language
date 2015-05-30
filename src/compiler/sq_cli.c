#include "sq_cli.h"

#include "string_helpers.h"
#include "list_helper.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static
CommandLineOptions * create_CommandLineOptions(){
    CommandLineOptions * options = (CommandLineOptions *)malloc(sizeof(CommandLineOptions));
    memset(options, 0, sizeof(CommandLineOptions));
    
    return options;
}
//Baseado em: http://stackoverflow.com/a/24479532
CommandLineOptions * sq_receiveArgs(int argc, char ** argv){
    CommandLineOptions * options = create_CommandLineOptions();
    
    size_t optind;
    for (optind = 1; optind < argc && argv[optind][0] == '-'; optind++) {
        switch (argv[optind][1]) {
        case 'o': 
            if(optind + 1 < argc){
                options->output = cpyString(argv[optind + 1]);
                break;
            }
        default:
            fprintf(stderr, "Usage: %s -o output_file < input_file.sq \n", argv[0]);
            exit(EXIT_FAILURE);
        }   
    } 
}

void sq_printStatus(SquirrelContext * sqContext){
    int errCount = sq_getErrorCount(sqContext);
    if(errCount > 0){
        fprintf(stderr, "\n\nCompilação falhou! Foram encontrados %d erros:\n\n", errCount);
        char * errListStr = joinList(sqContext->errorList, "\n", NULL);
        fprintf(stderr, "%s\n", errListStr);
        free(errListStr);
    }
}

//baseado em: http://hackerslife.blogspot.com.br/2005/01/write-to-file-in-c.html
void sq_writeFile(const char * programContent, const char * outputFile){
    FILE *file = fopen(outputFile,"w");
    if(file == NULL){
        fprintf(stderr, "Failed to open file '%s' to write program.\n", outputFile);
    }
    fprintf(file,"%s", programContent); /*escreve conteúdo do programa*/
    fclose(file); /*fechar arquivo*/
}