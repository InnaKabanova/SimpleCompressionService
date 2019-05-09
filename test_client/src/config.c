#include "config.h"
#include "utilities.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

#define TC_CONFIG_EXTRACT_CONTENTS_DEBUGGING
#define TC_CONFIG_FILES_LIST_DEBUGGING
#define TC_CONFIG_FILES_LIST_CONTENTS
#define TC_CONFIG_REQUESTS_FILE_DEBUGGING
#define TC_CONFIG_REQUESTS_FILE_CONTENTS

char* extract_contents(const char* filepath)
{
    long int contents_len = 0;
    FILE* handle = fopen(filepath, "r");
    if(NULL == handle)
    {
#ifdef TC_CONFIG_EXTRACT_CONTENTS_DEBUGGING
        printf("ERROR: failed to open file '%s'. Errno: %d.\n",
               filepath, errno);
#endif
        return 0;
    }

    fseek(handle, 0L, SEEK_END);
    contents_len = ftell(handle);
    fseek(handle, 0L, SEEK_SET);
    if(0 == contents_len)
    {
#ifdef TC_CONFIG_EXTRACT_CONTENTS_DEBUGGING
        printf("ERROR: file '%s' is empty.\n", filepath);
#endif
        return 0;
    }
    else if(-1L == contents_len)
    {
#ifdef TC_CONFIG_EXTRACT_CONTENTS_DEBUGGING
        printf("ERROR: failed to read '%s'. Errno: %d.\n",
               filepath, errno);
#endif
        return 0;
    }
#ifdef TC_CONFIG_EXTRACT_CONTENTS_DEBUGGING
    printf("Size of '%s': %li\n", filepath, contents_len);
#endif

    char* str = (char*)malloc(sizeof(char) * (contents_len + 1));
    *(str + contents_len) = '\0';
    fread((void*)str, contents_len, 1, handle);
    fclose(handle);
    return str;
}

int get_filepathes(char** files_string)
{
    if(NULL == files_string || NULL != *files_string)
    {
#ifdef TC_CONFIG_FILES_LIST_DEBUGGING
        printf("Files list processing | ERROR: bad argument.\n");
#endif
        return 0;
    }

#ifdef TC_CONFIG_FILES_LIST_DEBUGGING
        printf("Files list processing | Extracting.\n");
#endif
    *files_string = extract_contents(FILES_LIST_PATH);
    if(NULL == *files_string)
        return 0;

#ifdef TC_CONFIG_FILES_LIST_CONTENTS
    printf("Files list contents:\n%s\n", *files_string);
#endif
    return 1;
}

int get_requests(const char* filepath,
                 tc_internal_request_t** requests_chain)
{
    if(NULL == requests_chain || NULL != *requests_chain)
    {
#ifdef TC_CONFIG_REQUESTS_FILE_DEBUGGING
        printf("From %lu | '%s' | ERROR: bad argument.\n",
               pthread_self(), filepath);
#endif
        return 0;
    }

#ifdef TC_CONFIG_REQUESTS_FILE_DEBUGGING
        printf("From %lu | '%s' | Extracting.\n",
               pthread_self(), filepath);
#endif
    char* requests_string = extract_contents(filepath);
    if(NULL == requests_string)
        return 0;

#ifdef TC_CONFIG_REQUESTS_FILE_CONTENTS
    printf("From %lu | '%s' contents: \n%s\n",
           pthread_self(), filepath, requests_string);
#endif

//    char* token = NULL;
//    const char delim[2] = "\n";
//    token = strtok(requests_string, delim);
//    while(NULL != token)
//    {
//        printf("From %lu | Token: '%s'\n", pthread_self(), token);
//        token = strtok(NULL, delim);
//    }

    /*
     *  Positive cases:
     *
        token \n
        if(token's len == 1) simple request
        else
            take the 1st byte, write it as RC
            ignore the whitespaces after the first byte
        Assume the rest of the token is a payload
    */

    return 1;
}
