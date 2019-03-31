#ifndef SCS_REQUESTS_QUEUE
#define SCS_REQUESTS_QUEUE

#include "request.h"

#include <sys/socket.h>

typedef struct scs_internal_request
{
    request_header_t header;
    char** payload;
    struct sockaddr_storage client_addr;
    socklen_t client_addr_len;
    // TODO: socket
} scs_internal_request_t;

#endif /* SCS_REQUESTS_QUEUE */
