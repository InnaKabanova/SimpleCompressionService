#include "internal_request.h"

#include <stdlib.h>
#include <string.h>

tc_internal_request_t* get_basic_request()
{
    tc_internal_request_t* new_request =
    (tc_internal_request_t*)malloc(sizeof(tc_internal_request_t));
    new_request->header.magic_value = DEFAULT_MAGIC_VALUE;
    new_request->header.payload_length = 0;
    new_request->payload = NULL;
    new_request->next_request = NULL;
    return new_request;
}

tc_internal_request_t* create_request(const char* request_str)
{
    size_t request_len;
    if(NULL == request_str || 0 == (request_len = strlen(request_str)))
        return NULL;

    const char* ptr = request_str;
    tc_internal_request_t* new_request;

    // 1st & 2nd bytes determine whether request_str can be considered
    // valid.

    // Check candidates for simple requests (PING, GET_STATS, RESET_STATS)
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
                new_request->header.request_code = PING;
                break;
            case '2':
                new_request->header.request_code = GET_STATS;
                break;
            case '3':
                new_request->header.request_code = RESET_STATS;
                break;
            }
            return new_request;
        }
    }
    // Check candidates for requests with payload (COMPRESS)
    else if(*ptr == '4')
    {
        const char delim = *(ptr + 1);
        if(' ' != delim || '\0' == delim || '\n' == delim)
            return NULL;
        else
        {
            new_request = get_basic_request();
            new_request->header.request_code = COMPRESS;
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
                new_request->header.payload_length = payload_len;
                new_request->payload = (char*)malloc(payload_len);
                strncpy(new_request->payload, ptr, payload_len);
                return new_request;
            }
        }
    }
    else
        return NULL;
}
