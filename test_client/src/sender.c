#include "sender.h"
#include "utilities.h"

#include <stdlib.h>

void parse_req_data(const char* filepath);

void *send_requests(void* args)
{
    struct sender_args* req_data = (struct sender_args*)args;
    if(req_data->sock_descr <= 0 || req_data->filepath == NULL)
        return NULL; // TODO: return with an error code

    parse_req_data(req_data->filepath);
    // send_all(req_data->sock_descr, ...);
}

void parse_req_data(const char* filepath)
{
    // TODO
}

