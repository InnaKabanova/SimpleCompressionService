#include "internal_request.h"
#include "utilities.h"

int send_request(tc_internal_request_t* msg, const int sock_descr)
{
    unsigned int header_bytes_sent = sizeof(request_header_t);
    unsigned int payload_bytes_sent = msg->header.payload_len;

    msg->header.magic_value = htonl(msg->header.magic_value);
    msg->header.uuid = htonl(msg->header.magic_value);
    msg->header.code = htons(msg->header.code);
    msg->header.payload_len = htons(msg->header.payload_len);

    if(0 == send_all(sock_descr, (char*)(&msg->header),
                     &header_bytes_sent))
        return 0;

    if(payload_bytes_sent)
    {
        if(0 == send_all(sock_descr, (char*)(&msg->payload),
                         &payload_bytes_sent))
            return 0;
    }

    return 1;
}
