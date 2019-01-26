#include "sender.h"
#include "utilities.h"
#include "networking.h"

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>

#define TC_SENDER_DEBUGGING

static int parse_req_data(FILE* filepath);

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
    FILE* req_file;
    int sock_descr;

    req_file = fopen(thread_io->filepath, "r");
    if(!req_file)
    {
#ifdef TC_SENDER_DEBUGGING
        printf("From %lu | ERROR: failed to find/open a requests file. "
               "Errno: %d.\n", pthread_self(), errno);
#endif
        thread_io->exit_status = CONFIG_ERROR;
        pthread_exit((void*)(&thread_io->exit_status));
    }

    if(0 == parse_req_data(req_file))
    {
#ifdef TC_SENDER_DEBUGGING
        printf("From %lu | ERROR: failed to read requests from the file.\n",
               pthread_self());
#endif
        thread_io->exit_status = CONFIG_ERROR;
        pthread_exit((void*)(&thread_io->exit_status));
    }

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
    else
    {
#ifdef TC_SENDER_DEBUGGING
        printf("From %lu | Successfuly connected to the service.\n",
               pthread_self());
#endif
    }

    //----------------------------------------------------------------
    // Main work:
    //----------------------------------------------------------------






}

static int parse_req_data(FILE* filepath)
{
    return 1;
}

