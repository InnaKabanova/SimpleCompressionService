#include "config.h"
#include "utilities.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define TC_CONFIG_DEBUGGING
#define FILES_LIST_PATH "../config/files_list"

int get_filepathes(char** files_string)
{
    long int contents_len = 0;
    FILE* files_list = fopen(FILES_LIST_PATH, "r");
    if(NULL == files_list)
    {
#ifdef TC_CONFIG_DEBUGGING
        printf("ERROR: failed to open the files list. Errno: %d.\n",
               errno);
#endif
        files_string = NULL; return 0;
    }

    fseek(files_list, 0L, SEEK_END);
    contents_len = ftell(files_list);
    fseek(files_list, 0L, SEEK_SET);
    if(0 == contents_len)
    {
#ifdef TC_CONFIG_DEBUGGING
        printf("ERROR: the files list is empty.\n");
#endif
        files_string = NULL; return 0;
    }
    else if(-1L == contents_len)
    {
#ifdef TC_CONFIG_DEBUGGING
        printf("ERROR: failed to read the files list's contents. "
               "Errno: %d.\n", errno);
#endif
        files_string = NULL; return 0;
    }
#ifdef TC_CONFIG_DEBUGGING
    printf("Size of the found files list: %li\n", contents_len);
#endif

    *files_string = (char*)malloc(sizeof(char) * (contents_len + 1));
    (*files_string)[contents_len] = '\0';
    fread((void*)(*files_string), contents_len, 1, files_list);
    fclose(files_list);
#ifdef TC_CONFIG_DEBUGGING
    printf("String extracted from the config file:\n%s[EOF]\n",
           *files_string);
#endif

    return 1;
}
