#include "processor.h"

#include <pthread.h>

#define SCS_PROCESSOR_DEBUGGING

void* process_requests(void* args)
{
    if(!args)
        pthread_exit(NULL);
    processor_args_t* thread_io = (processor_args_t*)args;

    thread_io->exit_status = PRC_OK;
    pthread_exit((void*)(&thread_io->exit_status));
}
