#include "config.h"
#include "utilities.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define TC_CONFIG_DEBUGGING
#define CONFIG_FILE_PATH "../config/request_files"

int extract_config(char** files_string)
{
    FILE* f_read;
    long int buff_size;

#ifdef TC_CONFIG_DEBUGGING
    printf("Trying to read the config file...\n");
#endif

    f_read = fopen(CONFIG_FILE_PATH, "r");
    if(!f_read)
    {
#ifdef TC_CONFIG_DEBUGGING
        printf("ERROR: failed to open the file. Errno: %d.\n", errno);
#endif
        files_string == NULL;
        return 0;
    }

    fseek(f_read, 0L, SEEK_END);
    buff_size = ftell(f_read);
    fseek(f_read, 0L, SEEK_SET);
#ifdef TC_CONFIG_DEBUGGING
    printf("Size of the config file: %li\n", buff_size);
#endif
    if(0 == buff_size)
    {
        files_string == NULL;
        return 0;
    }

    *files_string = (char*)malloc(sizeof(char) * buff_size);
    if(NULL == *files_string)
    {
#ifdef TC_CONFIG_DEBUGGING
        printf("ERROR: failed to allocate buffer for config data.\n");
#endif
        files_string == NULL;
        return 0;
    }

    fread((void*)(*files_string), buff_size, 1, f_read);
    fclose(f_read);
#ifdef TC_CONFIG_DEBUGGING
    printf("String extracted from the config file:\n%s.\n", *files_string);
#endif
    return 1;
}
