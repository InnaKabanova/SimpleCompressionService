#include "sender.h"

#include <pthread.h>

#define SCS_SENDER_DEBUGGING

void* send_responses(void* args)
{
    if(!args)
        pthread_exit(NULL);
    sender_args_t* thread_io = (sender_args_t*)args;

    thread_io->exit_status = SND_OK;
    pthread_exit((void*)(&thread_io->exit_status));
}
