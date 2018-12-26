#include "config.h"
#include "utilities.h"

#include <stdio.h>
#include <stdlib.h>

#define DEBUGGING

char* parse_config_list()
{
    FILE* f_read;
    long int buff_size;
    char* files_string = NULL;

    f_read = fopen("../config/request_files", "r");
    if(!f_read)
        exit_with_failure("failed to open the files list\n");

    fseek(f_read, 0L, SEEK_END);
    buff_size = ftell(f_read);
#ifdef DEBUGGING
    printf("Size of 'request_files': %li\n", buff_size);
#endif
    fseek(f_read, 0L, SEEK_SET);

    files_string = (char*)malloc(sizeof(char) * buff_size);
    if(NULL == files_string)
        exit_with_failure("failed to allocate memory for files list");
    fread((void*)files_string, buff_size, 1, f_read);
#ifdef DEBUGGING
    printf("String extracted from 'request_files': %s.\n", files_string);
#endif

    fclose(f_read);
    return files_string;
}
