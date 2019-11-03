#ifndef SIMPLE_COMPR_SERVICE_REQUESTS_QUEUE
#define SIMPLE_COMPR_SERVICE_REQUESTS_QUEUE

#include "request.h"

typedef struct scs_internal_request
{
    request_header_t header;
    char** payload;
    int sock_descr;
} scs_internal_request_t;

void push_request(scs_internal_request_t* request);

#endif /* SIMPLE_COMPR_SERVICE_REQUESTS_QUEUE */
