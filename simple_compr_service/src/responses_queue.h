#ifndef SIMPLE_COMPR_SERVICE_RESPONSES_QUEUE
#define SIMPLE_COMPR_SERVICE_RESPONSES_QUEUE

#include "internal_response.h"

int init_responses_queue(int size);
int push_response_blocking(scs_internal_response_t* response,
                          int max_waiting_time_secs);
scs_internal_response_t* pop_response_blocking(void);

#endif /* SIMPLE_COMPR_SERVICE_RESPONSES_QUEUE */
