#include "internal_request.h"
#include "utilities.h"

int send_request(tc_internal_request_t* msg, const int sock_descr)
{
    unsigned int header_sz = sizeof(request_header_t);
    unsigned int payload_sz = msg->header.payload_len;

    msg->header.magic_value = htonl(msg->header.magic_value);
    msg->header.uuid = htonl(msg->header.uuid);
    msg->header.code = htons(msg->header.code);
    msg->header.payload_len = htons(msg->header.payload_len);

    if(0 == send_all(sock_descr, (char*)(&msg->header), &header_sz))
    {
        return 0;
    }

    if(payload_sz)
    {
        if(0 == send_all(sock_descr, msg->payload, &payload_sz))
            return 0;
    }

    return 1;
}
