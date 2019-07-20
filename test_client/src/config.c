#include "config.h"
#include "utilities.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

// Print errors when extracting contents of any file:
#define TC_CONFIG_EXTRACTION_DBG
// Print debug info when reading FILES_LIST:
#define TC_CONFIG_FILES_LIST_DBG
// Print contents of FILES_LIST (if successfully extracted):
#define TC_CONFIG_FILES_LIST_CONTENTS
// Print debug info when reading any file with serialized requests:
#define TC_CONFIG_REQUESTS_FILE_DBG
// Print contents of any file with requests (if successfully extracted):
#define TC_CONFIG_REQUESTS_FILE_CONTENTS

char* extract_contents(const char* filepath)
{
    long int contents_len = 0;
    FILE* handle = fopen(filepath, "r");
    if(NULL == handle)
    {
#ifdef TC_CONFIG_EXTRACTION_DBG
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
#ifdef TC_CONFIG_EXTRACTION_DBG
        printf("ERROR: file '%s' is empty.\n", filepath);
#endif
        return 0;
    }
    else if(-1L == contents_len)
    {
#ifdef TC_CONFIG_EXTRACTION_DBG
        printf("ERROR: failed to read '%s'. Errno: %d.\n",
               filepath, errno);
#endif
        return 0;
    }
#ifdef TC_CONFIG_EXTRACTION_DBG
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
#ifdef TC_CONFIG_FILES_LIST_DBG
        printf("Files list processing | ERROR: bad argument.\n");
#endif
        return 0;
    }

#ifdef TC_CONFIG_FILES_LIST_DBG
        printf("Files list processing | Extracting contents.\n");
#endif
    *files_string = extract_contents(FILES_LIST);
    if(NULL == *files_string)
        return 0;

#ifdef TC_CONFIG_FILES_LIST_CONTENTS
    printf("Contents of the files list:\n%s\n", *files_string);
#endif
    return 1;
}

int get_requests(const char* filepath,
                 tc_internal_request_t** requests_chain)
{
    if(!filepath || !requests_chain || NULL != *requests_chain)
    {
#ifdef TC_CONFIG_REQUESTS_FILE_DBG
        printf("From %lu | '%s' | ERROR: bad argument.\n",
               pthread_self(), filepath);
#endif
        return 0;
    }

#ifdef TC_CONFIG_REQUESTS_FILE_DBG
    printf("From %lu | '%s' | Extracting contents.\n",
           pthread_self(), filepath);
#endif
    char* requests_string = extract_contents(filepath);
    if(NULL == requests_string)
    {
        return 0;
    }
#ifdef TC_CONFIG_REQUESTS_FILE_CONTENTS
    printf("From %lu | Contents of '%s': \n%s\n",
           pthread_self(), filepath, requests_string);
#endif

    int ret = 0;
    unsigned int requests_num = 0;
    tc_internal_request_t* first = NULL;
    tc_internal_request_t* last = NULL;
    char* token = NULL;
    char* context;
    const char delim[2] = "\n";

    // Keep trying to create a first request until valid one is
    // available or there are no more tokens to process.
    token = strtok_r(requests_string, delim, &context);
    if(NULL == token)
        goto exit;
    do
    {
#ifdef TC_CONFIG_REQUESTS_FILE_CONTENTS
        printf("From %lu | '%s' | Token: '%s'\n",
               pthread_self(), filepath, token);
#endif
        first = create_request(token);
        if(NULL == (token = strtok_r(NULL, delim, &context))) break;
    }
    while(NULL == first);
    if(NULL == first)
        goto exit;
    else
    {
        requests_num++;
        requests_chain = &first;
        last = first;
        ret = 1;
    }

    // Process next tokens, if any:
    while(NULL != token)
    {
#ifdef TC_CONFIG_REQUESTS_FILE_CONTENTS
        printf("From %lu | '%s' | Token: '%s'\n",
               pthread_self(), filepath, token);
#endif
        tc_internal_request_t* new = create_request(token);
        if(NULL != new)
        {
            last->next_request = new;
            last = new;
            requests_num++;
        }
        token = strtok_r(NULL, delim, &context);
    }

exit:
#ifdef TC_CONFIG_REQUESTS_FILE_DBG
    printf("From %lu | '%s' | Valid requests created: %d\n",
           pthread_self(), filepath, requests_num);
#endif
    free(requests_string);
    return ret;
}
