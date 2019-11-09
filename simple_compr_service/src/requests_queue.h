#ifndef SIMPLE_COMPR_SERVICE_REQUESTS_QUEUE
#define SIMPLE_COMPR_SERVICE_REQUESTS_QUEUE

#include "internal_request.h"

int init_requests_queue(int size);
int push_request_blocking(scs_internal_request_t* request,
                          int max_waiting_time_secs);
scs_internal_request_t* pop_request_blocking(void);

#endif /* SIMPLE_COMPR_SERVICE_REQUESTS_QUEUE */
