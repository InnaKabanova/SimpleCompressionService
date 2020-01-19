#ifndef SIMPLE_COMPR_SERVICE_INTERNAL_RESPONSE
#define SIMPLE_COMPR_SERVICE_INTERNAL_RESPONSE

#include "response.h"

typedef struct scs_internal_response
{
    response_header_t header;
    void* result;
    int sock_descr;
} scs_internal_response_t;

scs_internal_response_t* create_basic_response(uint32_t uuid, int sock_descr);

int send_response(scs_internal_response_t* msg);

#endif /* SIMPLE_COMPR_SERVICE_INTERNAL_RESPONSE */
