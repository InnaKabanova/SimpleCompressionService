#include "sender.h"
#include "utilities.h"
#include "networking.h"
#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

#define TC_SENDER_DEBUGGING

void* send_requests(void* args)
{
    if(!args)
        pthread_exit(NULL);
    sender_args_t* thread_io = (sender_args_t*)args;

    //----------------------------------------------------------------
    // Initialization:
    //----------------------------------------------------------------
    int sock_descr;
    tc_internal_request_t* requests_chain = NULL;

    if(0 == get_requests(thread_io->filepath, &requests_chain))
    {
#ifdef TC_SENDER_DEBUGGING
        printf("From %lu | ERROR: failed to get requests from "
               "config file '%s'.\n", pthread_self(), thread_io->filepath);
#endif
        thread_io->exit_status = REQUESTS_FILE_ERROR;
        pthread_exit((void*)(&thread_io->exit_status));
    }

    sock_descr = try_to_connect(thread_io->node, thread_io->port_num);
    if(-1 == sock_descr)
    {
#ifdef TC_SENDER_DEBUGGING
        printf("From %lu | ERROR: failed to connect to the service "
               "%s on port %s.\n", pthread_self(), thread_io->node,
               thread_io->port_num);
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
}



