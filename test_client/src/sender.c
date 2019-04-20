#include "sender.h"
#include "utilities.h"
#include "networking.h"
#include "request.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

#define TC_SENDER_DEBUGGING

typedef struct tc_internal_request
{
    request_header_t header;
    char* payload;
    struct tc_internal_request* next_request;
} tc_internal_request_t;

static sender_exit_status_t parse_req_data
(const char* filepath, tc_internal_request_t* tc_internal_request);

void* send_requests(void* args)
{
    if(!args)
        pthread_exit(NULL);

    sender_args_t* thread_io = (sender_args_t*)args;

#ifdef TC_SENDER_DEBUGGING
    printf("From %lu | File: '%s'.\n", pthread_self(), thread_io->filepath);
#endif

    //----------------------------------------------------------------
    // Initialization:
    //----------------------------------------------------------------
    int sock_descr;
    tc_internal_request_t* request = NULL; // chain of requests

    thread_io->exit_status = parse_req_data(thread_io->filepath, request);
    if(OK != thread_io->exit_status)
        pthread_exit((void*)(&thread_io->exit_status));

    sock_descr = try_to_connect(thread_io->node, thread_io->port_num);
    if(-1 == sock_descr)
    {
#ifdef TC_SENDER_DEBUGGING
        printf("From %lu | ERROR: failed to connect to the service.\n",
               pthread_self());
#endif
        thread_io->exit_status = CONNECTION_ERROR;
        pthread_exit((void*)(&thread_io->exit_status));
    }
#ifdef TC_SENDER_DEBUGGING
        printf("From %lu | Successfuly connected to the service.\n",
               pthread_self());
#endif

    //----------------------------------------------------------------
    // Main work:
    //----------------------------------------------------------------

    thread_io->exit_status = OK;
    pthread_exit((void*)(&thread_io->exit_status));

//    while(request->next_request != NULL)
//    {


//    }

    // TODO: don't forget to free memory of requests' list and their payload !

    thread_io->exit_status = OK;
    pthread_exit((void*)(&thread_io->exit_status));
}

static sender_exit_status_t parse_req_data
(const char* filepath, tc_internal_request_t* request)
{
#ifdef TC_SENDER_DEBUGGING
    printf("From %lu (parse_req_data) | File: '%s'.\n", pthread_self(), filepath);
#endif

    FILE* req_file = fopen(filepath, "r");
    if(request != NULL)
    {
#ifdef TC_SENDER_DEBUGGING
        printf("From %lu | ERROR: bad argument.\n", pthread_self());
#endif
        return OTHER_ERROR;
    }
    if(!req_file)
    {
#ifdef TC_SENDER_DEBUGGING
        printf("From %lu | ERROR: failed to find/open a requests file "
               "Errno: %d.\n", pthread_self(), errno);
#endif
        return CONFIG_FILE_ERROR;
    }

//    request = (tc_internal_request_t*)malloc(sizeof(tc_internal_request_t));
//    request->header.magic_value = DEFAULT_MAGIC_VALUE;
//    request->header.payload_length = 0;
//    request->header.request_code = 1;
//    request->payload = NULL;
//    request->next_request = (tc_internal_request_t*)malloc(sizeof(tc_internal_request_t));
//    tc_internal_request_t* request_2 = request->next_request;
//    request_2->header.magic_value = DEFAULT_MAGIC_VALUE;
//    request_2->header.payload_length = 5;
//    request_2->header.request_code = 4;
//    request_2->payload = malloc(sizeof(request_2->header.payload_length));
//    strncpy(request_2->payload, "test", request_2->header.payload_length);
//    request_2->next_request = NULL;

// TODO: implement the real config file parsing instead of hardcoding
// the requests

//#ifdef TC_SENDER_DEBUGGING
//        printf("From %lu | ERROR: failed to read requests from the file.\n",
//               pthread_self());
//#endif
// return CONFIG_PARSING_ERROR

    return OK;
}

// TODO: cleanup needed
// pthread_cleanup_push(), pthread_cleanup_pop()


