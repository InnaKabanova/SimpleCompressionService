#ifndef TEST_CLIENT_INTERNAL_REQUEST
#define TEST_CLIENT_INTERNAL_REQUEST

#include "request.h"

typedef struct tc_internal_request
{
    request_header_t header;
    char* payload;
    struct tc_internal_request* next_request;
} tc_internal_request_t;

int send_request(tc_internal_request_t* msg, const int sock_descr);

#endif /* TEST_CLIENT_INTERNAL_REQUEST */
