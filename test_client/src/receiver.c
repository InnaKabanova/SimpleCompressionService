#include "receiver.h"

#include <stdio.h>
#include <pthread.h>

void* receive_requests(void* args)
{
    if(NULL == args) return NULL;

    int* sock_descr = (int*)args;

    printf("From '%lu' | RECEIVE_REQUESTS: '%d'\n",
           pthread_self(), *sock_descr);

    *sock_descr = RECV_SUCCESS;

    pthread_exit(args);
}

