#include "requests_import.h"
#include "uuid.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

//--------------------------------------------------------------------
// Requests importer configuration:

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
//--------------------------------------------------------------------

tc_internal_request_t* get_basic_request()
{
    tc_internal_request_t* new_request =
    (tc_internal_request_t*)malloc(sizeof(tc_internal_request_t));
    new_request->header.magic_value = REQUEST_MAGIC_VALUE;
    get_uuid(&new_request->header.uuid);
    new_request->header.payload_len = 0;
    new_request->payload = NULL;
    new_request->next_request = NULL;
    return new_request;
}

/**
 * @brief Deserializes provided textual data into internal
 * represenatation of a request. Memory for each request is dynamically
 * allocated and responsibility of its freeing is on the function's
 * caller.
 * @param[in] request_str: textual data representing a request.
 * @return valid (tc_internal_request_t*) pointer in case of success,
 * NULL in case of failure.
 */
tc_internal_request_t* create_request(const char* request_str)
{
    size_t request_len;
    if(NULL == request_str || 0 == (request_len = strlen(request_str)))
        return NULL;

    const char* ptr = request_str;
    tc_internal_request_t* new_request;

    // 1st & 2nd bytes determine whether request_str can be considered
    // valid.

    // Check candidates for becoming simple requests
    // (without payload):
    if(*ptr == '1' || *ptr == '2' || *ptr == '3')
    {
        if(1 != request_len)
            return NULL;
        else
        {
            new_request = get_basic_request();
            switch(*ptr)
            {
            case '1':
                new_request->header.code = REQ_PING;
                break;
            case '2':
                new_request->header.code = REQ_GET_STATS;
                break;
            case '3':
                new_request->header.code = REQ_RESET_STATS;
                break;
            }
            return new_request;
        }
    }
    // Check candidates for becoming requests with payload:
    else if(*ptr == '4')
    {
        const char delim = *(ptr + 1);
        if(' ' != delim || '\0' == delim || '\n' == delim)
            return NULL;
        else
        {
            new_request = get_basic_request();
            new_request->header.code = REQ_COMPRESS;
            // Ignore whitespaces:
            while(' ' == *++ptr);
            // Assume the rest is a payload:
            size_t payload_len = strlen(ptr);
            if(0 == payload_len)
            {
                free(new_request);
                return NULL;
            }
            else
            {
                new_request->header.payload_len = payload_len;
                new_request->payload = (char*)malloc(payload_len);
                strncpy(new_request->payload, ptr, payload_len);
                return new_request;
            }
        }
    }
    else
        return NULL;
}

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

int get_config_filepathes(char** files_string)
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

int import_requests(const char* filepath,
                 tc_internal_request_t** requests_chain)
{
    if(!filepath || NULL == requests_chain || NULL != *requests_chain)
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
        *requests_chain = first;
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

