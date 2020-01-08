#include "sender.h"
#include "connection.h"

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <sys/socket.h>

#ifdef REQUESTS_IMPORT_MODE
#include "requests_import.h"

#else // REQUESTS_GENERATION_MODE
#include "requests_generation.h"
#endif

extern pthread_key_t SOCK_DESCR_KEY;

#ifdef REQUESTS_IMPORT_MODE
void process_chain(tc_internal_request_t* requests_chain,
                   const int sock_descr);

#else // REQUESTS_GENERATION_MODE
void process_chain(test_action_t* test_actions_chain);
#endif

void* send_requests(void* args)
{
    if(!args) pthread_exit(NULL);

    sender_args_t* thread_io = (sender_args_t*)args;

    thread_io->sock_descr = try_to_connect(thread_io->node,
                                           thread_io->port_num);
    if(-1 == thread_io->sock_descr)
    {
        printf("From '%lu' | ERROR: failed to connect to the service "
               "'%s' on port '%s'.\n",
               pthread_self(), thread_io->node, thread_io->port_num);
        thread_io->exit_status = SND_CONNECTION_ERROR;
        goto exit;
    }

    if(0 != pthread_setspecific(SOCK_DESCR_KEY,
                                (const void*)(&thread_io->sock_descr)))
    {
        printf("From '%lu' | ERROR: failed to save thread-specific "
               "connection descriptor.\n",
               pthread_self());
        thread_io->exit_status = SND_THREAD_ERROR;
        goto exit;
    }

    printf("From '%lu' | Successfully connected to the service.\n",
           pthread_self());

#ifdef REQUESTS_IMPORT_MODE
    tc_internal_request_t* requests_chain = NULL;
    if(0 == import_requests(thread_io->config_path, &requests_chain))
    {
        printf("From '%lu' | ERROR: failed to import requests from "
               "config file '%s'.\n",
               pthread_self(), thread_io->config_path);
        thread_io->exit_status = SND_REQUESTS_IMPORT_ERROR;
        goto exit;
    }
    else
    {
        process_chain(requests_chain, thread_io->sock_descr);
        thread_io->exit_status = SND_SUCCESS;
    }

#else // REQUESTS_GENERATION_MODE
    test_action_t* tests_chain = NULL;
    if(NULL == (tests_chain = generate_requests(
                    TS_VALID_REQUESTS_WITH_PAYLOAD)))
    {
        printf("From '%lu' | ERROR: failed to generate requests.\n",
               pthread_self());
        thread_io->exit_status = SND_REQUESTS_GENERATION_ERROR;
    }
    else
    {
        process_chain(tests_chain);
        thread_io->exit_status = SND_SUCCESS;
    }
#endif

exit:
    pthread_exit((void*)(&thread_io->exit_status));
}

#ifdef REQUESTS_IMPORT_MODE
void process_chain(tc_internal_request_t* requests_chain,
                   const int sock_descr)
{
    if(NULL == requests_chain) return;

    tc_internal_request_t* curr_request = requests_chain;
    tc_internal_request_t* disposed_request = NULL;
    while(NULL != curr_request)
    {
        if(0 == send_request(curr_request, sock_descr))
        {
            printf("From '%lu' | ERROR: could not send a request: "
                   "[%d].\n",
                   pthread_self(), errno);
        }

        if(NULL != curr_request->payload)
            free(curr_request->payload);
        disposed_request = curr_request;
        curr_request = curr_request->next_request;
        free(disposed_request);

        if(NULL == curr_request)
            shutdown(sock_descr, SHUT_WR);
    }
}

#else // REQUESTS_GENERATION_MODE
void process_chain(test_action_t* test_actions_chain)
{
    if(NULL == test_actions_chain) return;

    test_action_t* curr_ta = test_actions_chain;
    test_action_t* disposed_ta = NULL;
    while(NULL != curr_ta)
    {
        printf("From '%lu' | Performing test action: '%s'.\n",
               pthread_self(), curr_ta->descr);
        if(NULL != curr_ta->action)
            curr_ta->action();
        disposed_ta = curr_ta;
        curr_ta = curr_ta->next_test_action;
        free(disposed_ta);
    }
}

#endif

