#include "receiver.h"

#include <stdio.h>
#include <pthread.h>

const char* receiver_exit_status_2_str(receiver_exit_status_t status)
{
    switch (status)
    {
        case RECV_SUCCESS: return "RECV_SUCCESS";
    }
}

void* receive_responses(void* args)
{
    if(NULL == args) return NULL;

    int* sock_descr = (int*)args;

    printf("From '%lu' | RECEIVE_RESPONSES: sock_descr: '%d'\n",
           pthread_self(), *sock_descr);

    *sock_descr = RECV_SUCCESS;

    pthread_exit(args);
}

