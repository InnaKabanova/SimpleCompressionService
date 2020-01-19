#include "internal_response.h"
#include "utilities.h"

#include <stdlib.h>

scs_internal_response_t* create_basic_response(uint32_t uuid, int sock_descr)
{
    scs_internal_response_t* new_response =
            (scs_internal_response_t*)malloc(sizeof(scs_internal_response_t));

    new_response->header.uuid = uuid;
    new_response->sock_descr = sock_descr;
    new_response->header.magic_value = RESPONSE_MAGIC_VALUE;
    new_response->header.status = RESP_OK;
    new_response->header.payload_len = 0;
    new_response->result = NULL;

    return new_response;
}

int send_response(scs_internal_response_t* msg)
{
    unsigned int header_sz = sizeof(response_header_t);
    unsigned int payload_sz = msg->header.payload_len;

    msg->header.magic_value = htonl(msg->header.magic_value);
    msg->header.uuid = htonl(msg->header.uuid);
    msg->header.status = htons(msg->header.status);
    msg->header.payload_len = htons(msg->header.payload_len);

    if(0 == send_all(msg->sock_descr, (char*)(&msg->header), &header_sz))
    {
        return 0;
    }

    if(payload_sz)
    {
        if(0 == send_all(msg->sock_descr, (char*)msg->result, &payload_sz))
            return 0;
    }

    return 1;
}
