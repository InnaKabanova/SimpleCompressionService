#include "sender.h"
#include "utilities.h"
#include "networking.h"

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define TC_SENDER_DEBUGGING

void parse_req_data(const char* filepath);

void* send_requests(void* args)
{
    struct sender_args* req_data = (struct sender_args*)args;
    // TODO: do the checks for input parameters here

#ifdef TC_SENDER_DEBUGGING
    printf("From %lu | File: '%s'.\n", pthread_self(), req_data->filepath);
#endif

    int sock_descr = try_to_connect(req_data->node, req_data->port_num);
    if(-1 == sock_descr)
    {
        printf("From %lu | Everything's bad.\n", pthread_self());
    }
    else
    {
        printf("From %lu | Successfuly connected.\n", pthread_self());
    }

}

void parse_req_data(const char* filepath)
{
    // TODO
}

