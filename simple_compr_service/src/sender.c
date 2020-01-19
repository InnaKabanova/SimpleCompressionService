#include "sender.h"
#include "responses_queue.h"

#include <pthread.h>
#include <sys/syslog.h>

#define SCS_SENDER_DBG

#ifdef SCS_SENDER_DBG
#include "logging_utilities.h"
#endif

void* send_responses(void* args)
{
    while (1)
    {
        scs_internal_response_t* response = pop_response_blocking();
        if(NULL == response) continue;

#ifdef SCS_SENDER_DBG
         log_response(response);
#endif /* SCS_PROCESSOR_DBG */

        if(!send_response(response)) log_sending_error(response->header.uuid);

        if(response->result != NULL) free(response->result);
        free(response);
    }
}
