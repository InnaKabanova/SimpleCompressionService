#ifndef SCS_REQUESTS_QUEUE
#define SCS_REQUESTS_QUEUE

#include "request.h"

#include <sys/socket.h>

typedef struct internal_request
{
    request_header_t header;
    char** payload;
    struct sockaddr_storage client_addr;
    socklen_t client_addr_len;
    // TODO: socket
} internal_request_t;

static const internal_request_t template_internal_request =
{.header = template_request_header};


#endif /* SCS_REQUESTS_QUEUE */
