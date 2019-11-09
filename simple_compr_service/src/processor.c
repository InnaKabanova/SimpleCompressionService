#include "processor.h"
#include "requests_queue.h"
#include "logging_utilities.h"

#include <stdlib.h>
#include <pthread.h>

void* process_requests(void* args)
{
    while (1)
    {
        scs_internal_request_t* request = pop_request_blocking();
        if(NULL != request) log_request(request);
        free(request);
    }
}
