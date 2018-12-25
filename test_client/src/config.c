#include "config.h"
#include "utilities.h"

#include <stdio.h>
#include <stdlib.h>

#define DEBUGGING

void parse_config_list()
{
    FILE* f_read;
    long int buff_size;
    char* configs = NULL;

    f_read = fopen("../config/request_files", "r");
    if(!f_read)
        exit_with_failure("failed to open the configs list\n");

    fseek(f_read, 0L, SEEK_END);
    buff_size = ftell(f_read);
#ifdef DEBUGGING
    printf("Size of the configs list file: %li\n", buff_size);
#endif
    fseek(f_read, 0L, SEEK_SET);

    configs = (char*)malloc(sizeof(char) * buff_size);
    if(configs == NULL)
    exit_with_failure("failed to allocate memory for configs list");

// TODO:
//    char str[80] = "This is - www.tutorialspoint.com - website";
//       const char s[2] = "-";
//       char *token;

//       /* get the first token */
//       token = strtok(str, s);

//       /* walk through other tokens */
//       while( token != NULL ) {
//          printf( " %s\n", token );

//          token = strtok(NULL, s);
//       }


    free(configs);
    fclose(f_read);


}
