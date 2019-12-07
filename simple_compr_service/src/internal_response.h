#ifndef SIMPLE_COMPR_SERVICE_INTERNAL_RESPONSE
#define SIMPLE_COMPR_SERVICE_INTERNAL_RESPONSE

#include "response.h"

typedef struct scs_internal_response
{
    response_header_t header;
    char* payload;
    int sock_descr;
} scs_internal_response_t;

#endif /* SIMPLE_COMPR_SERVICE_INTERNAL_RESPONSE */
