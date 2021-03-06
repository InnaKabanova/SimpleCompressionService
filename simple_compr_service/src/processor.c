#include "processor.h"
#include "requests_queue.h"
#include "responses_queue.h"

#include <stdlib.h>
#include <pthread.h>

#define SCS_PROCESSOR_DBG

#ifdef SCS_PROCESSOR_DBG
#include "logging_utilities.h"
#endif

/**
 * @brief Max. amount of time to wait for the 'Ping' response instance
 * to be pushed to the response queue before it's discarded and so
 * the corresponding 'Ping' request is considered dropped.
 */
#define MAX_PING_RESPONSE_WAITING_TIME_SEC 5

static void process_req_ping(scs_internal_request_t* request);
static void process_req_getstats(scs_internal_request_t* request);
static void process_req_resetstats(scs_internal_request_t* request);
static void process_req_compress(scs_internal_request_t* request);

void* process_requests(void* args)
{
    while (1)
    {
        scs_internal_request_t* request = pop_request_blocking();
        if(NULL == request) continue;

#ifdef SCS_PROCESSOR_DBG
         log_request(request);
#endif /* SCS_PROCESSOR_DBG */

         switch (request->header.code) {
         case REQ_PING:
             process_req_ping(request);
             break;
         case REQ_GET_STATS:
             process_req_getstats(request);
             break;
         case REQ_RESET_STATS:
             process_req_resetstats(request);
             break;
         case REQ_COMPRESS:
             process_req_compress(request);
             break;
         default:
             break;
         }

        if(request->payload != NULL) free(request->payload);
        free(request);
    }
}

static void process_req_ping(scs_internal_request_t* request)
{
    if(NULL == request) return;

    scs_internal_response_t* new_response = create_basic_response(
                request->header.uuid, request->sock_descr);
    push_response_blocking(new_response, MAX_PING_RESPONSE_WAITING_TIME_SEC);

}

static void process_req_getstats(scs_internal_request_t* request)
{
    if(NULL == request) return;

    scs_internal_response_t* new_response = create_basic_response(
                request->header.uuid, request->sock_descr);
    push_response_blocking(new_response, MAX_PING_RESPONSE_WAITING_TIME_SEC);
}

static void process_req_resetstats(scs_internal_request_t* request)
{
    if(NULL == request) return;

    scs_internal_response_t* new_response = create_basic_response(
                request->header.uuid, request->sock_descr);
    push_response_blocking(new_response, MAX_PING_RESPONSE_WAITING_TIME_SEC);
}

static void process_req_compress(scs_internal_request_t* request)
{
    if(NULL == request) return;

    scs_internal_response_t* new_response = create_basic_response(
                request->header.uuid, request->sock_descr);
    
//    new_response->header.status = RESP_OK;
//    new_response->header.payload_len = 0;
//    new_response->result = NULL;
    
    
    
    push_response_blocking(new_response, MAX_PING_RESPONSE_WAITING_TIME_SEC);
}
