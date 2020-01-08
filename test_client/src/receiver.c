#include "receiver.h"

#include <stdio.h>
#include <pthread.h>

void* receive_responses(void* args)
{
    if(NULL == args) return NULL;

    int* sock_descr = (int*)args;

    printf("From '%lu' | RECEIVE_RESPONSES: sock_descr: '%d'\n",
           pthread_self(), *sock_descr);

    *sock_descr = RECV_SUCCESS;

    pthread_exit(args);
}

