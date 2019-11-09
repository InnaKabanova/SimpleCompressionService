#ifndef SIMPLE_COMPR_SERVICE_INTERNAL_REQUEST
#define SIMPLE_COMPR_SERVICE_INTERNAL_REQUEST

#include "request.h"

typedef struct scs_internal_request
{
    request_header_t header;
    char* payload;
    int sock_descr;
} scs_internal_request_t;

#endif /* SIMPLE_COMPR_SERVICE_INTERNAL_REQUEST */
