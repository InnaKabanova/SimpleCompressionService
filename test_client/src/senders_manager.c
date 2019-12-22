#include "main_config.h"
#include "senders_manager.h"
#include "sender.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

pthread_key_t SOCK_DESCR_KEY;

static int join_senders(pthread_t* senders_pool, unsigned int num);

int run_senders(const char* node, const char* port_num)
{
    pthread_t senders_pool[MAX_SENDERS_NUM];
    sender_args_t args_pool[MAX_SENDERS_NUM];
    unsigned int it = 0;

    if(0 != pthread_key_create(&SOCK_DESCR_KEY, NULL))
        return 0;

#ifdef REQUESTS_IMPORT_MODE
    char* filepathes = NULL;
    char* token = NULL;
    char* context = NULL;
    const char delim[2] = "\n";

    if(0 == get_config_filepathes(&filepathes)) return 0;

    token = strtok_r(filepathes, delim, &context);
    while(NULL != token)
    {
        args_pool[it].config_path = token;
#else // REQUESTS_GENERATION_MODE
    for(it = 0; it < MAX_SENDERS_NUM; it++)
    {
#endif

        args_pool[it].node = node;
        args_pool[it].port_num = port_num;
        args_pool[it].exit_status = SND_BAD_ARGS;
        int ret = pthread_create(&senders_pool[it], NULL,
                                 send_requests, &args_pool[it]);
        if(0 == ret)
            printf("Created a worker thread with ID '%lu'.\n",
                   senders_pool[it]);
        else
            printf("ERROR: failed to create a sender thread: [%d]. "
                   "Contents of ID: '%lu'.\n",
                   ret, senders_pool[it]);

#ifdef REQUESTS_IMPORT_MODE
        it++;
        token = strtok_r(NULL, delim, &context);
        if(it == MAX_SENDERS_NUM)
        {
            join_senders(senders_pool, it);
            it = 0;
        }
    }
    free(filepathes);
#else // REQUESTS_GENERATION_MODE
    }
#endif

    join_senders(senders_pool, it);
    pthread_key_delete(SOCK_DESCR_KEY);
    return 1;
}

static int join_senders(pthread_t* senders_pool, unsigned int num)
{
    void* exit_status;
    for(unsigned int i = 0; i < num; i++)
    {
        int ret = pthread_join(senders_pool[i], &exit_status);
        if(0 == ret)
            printf("Thread '%lu' joined. Exit status: '%d'.\n",
                   senders_pool[i], *((sender_exit_status_t*)exit_status));
        else
            printf("ERROR: failed to join a sender thread with "
                   "ID '%lu': [%d].\n",
                   senders_pool[i], ret);
    }
    return 1;
}
