#ifndef TEST_CLIENT_INTERNAL_REQUEST
#define TEST_CLIENT_INTERNAL_REQUEST

#include "request.h"

typedef struct tc_internal_request
{
    request_header_t header;
    char* payload;
    struct tc_internal_request* next_request;
} tc_internal_request_t;

/**
 * @brief create_request. Deserializes provided textual data into
 * internal represenatation of a request. Memory for each request is
 * dynamically allocated and responsibility of its freeing is on the
 * function's caller.
 * @param[in] request_str: textual data representing a request.
 * @return valid (tc_internal_request_t*) pointer in case of success,
 * NULL in case of failure.
 */
tc_internal_request_t* create_request(const char* request_str);

#endif /* TEST_CLIENT_INTERNAL_REQUEST */
