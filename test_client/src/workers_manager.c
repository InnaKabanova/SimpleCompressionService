#include "main_config.h"
#include "workers_manager.h"
#include "sender.h"
#include "receiver.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

pthread_key_t SOCK_DESCR_KEY;

static sender_result_t* find_last_in_list(sender_result_t* results);
static int join_senders_concat_results(sender_result_t** results,
                                       pthread_t* senders_pool,
                                       sender_args_t* args_pool,
                                       unsigned int num);
static sender_result_t* join_senders(pthread_t* senders_pool,
                                     sender_args_t* args_pool,
                                     unsigned int num);
static int join_receivers(pthread_t* receivers_pool,
                          unsigned int num);

sender_result_t* run_senders(const char* node, const char* port_num)
{
    pthread_t senders_pool[MAX_SENDERS_NUM];
    sender_args_t args_pool[MAX_SENDERS_NUM];
    sender_result_t* results = NULL;
    unsigned int it = 0;

    if(0 != pthread_key_create(&SOCK_DESCR_KEY, NULL)) return results;

#ifdef REQUESTS_IMPORT_MODE
    char* filepathes = NULL;
    char* token = NULL;
    char* context = NULL;
    const char delim[2] = "\n";

    if(0 == get_config_filepathes(&filepathes)) return results;

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
        args_pool[it].sock_descr = -1;
        args_pool[it].exit_status = SND_BAD_ARGS;
        int ret = pthread_create(&senders_pool[it], NULL,
                                 send_requests, &args_pool[it]);
        if(0 == ret)
            printf("Created a sender thread with ID '%lu'.\n",
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
            join_senders_concat_results(&results, senders_pool,
                                        args_pool, it);
            it = 0;
        }
    }
    free(filepathes);
#else // REQUESTS_GENERATION_MODE
    }
#endif

    join_senders_concat_results(&results, senders_pool, args_pool, it);
    pthread_key_delete(SOCK_DESCR_KEY);
    return results;
}

static sender_result_t* find_last_in_list(sender_result_t* results)
{
    if(NULL == results) return NULL;

     sender_result_t* last = results;
     while(last->next != NULL)
         last = last->next;
     return last;
}

static int join_senders_concat_results(sender_result_t** results,
                                       pthread_t* senders_pool,
                                       sender_args_t* args_pool,
                                       unsigned int num)
{
    if(NULL == results) return 0;

    if(*results != NULL)
    {
        sender_result_t* last = find_last_in_list(*results);
        last->next = join_senders(senders_pool, args_pool, num);
    }
    else
    {
        *results = join_senders(senders_pool, args_pool, num);
    }
    return 1;
}


static sender_result_t* join_senders(pthread_t* senders_pool,
                                     sender_args_t* args_pool,
                                     unsigned int num)
{
    if(NULL == senders_pool || NULL == args_pool) return 0;

    void* exit_status = NULL;
    sender_result_t* results = NULL; // chain of results to return
    sender_result_t* last = NULL;

    for(unsigned int i = 0; i < num; i++)
    {
        int ret = pthread_join(senders_pool[i], &exit_status);
        if(0 == ret && exit_status != NULL)
        {
            printf("Sender thread '%lu' joined. Exit status: '%d'.\n",
                   senders_pool[i], *((sender_exit_status_t*)exit_status));

            // If a sender thread did its job successfully,...
            if(SND_SUCCESS == *((sender_exit_status_t*)exit_status))
            {
                // ... we retreive the information needed for a receiver thread,
                // which will take over the work on the corresponding connection.
                if(NULL == results)
                {
                    results = (sender_result_t*)malloc(sizeof(sender_result_t));
                    last = results;
                }
                else
                {
                    last->next = (sender_result_t*)malloc(sizeof(sender_result_t));
                    last = last->next;
                }
                last->sock_descr = args_pool[i].sock_descr;
                last->next = NULL;
            }
        }
        else
        {
            printf("ERROR: failed to join/run a sender thread with "
                   "ID '%lu': [%d].\n",
                   senders_pool[i], ret);
        }
    }

    return results;
}

int run_receivers(sender_result_t* results)
{
    if(NULL == results) return 0;

    pthread_t receivers_pool[MAX_RECEIVERS_NUM];

    sender_result_t* curr_result = results;
    int receivers_indx = 0;

    do
    {
        int ret = pthread_create(&receivers_pool[receivers_indx], NULL,
                                 receive_responses,
                                 (void*)(&curr_result->sock_descr));
        if(0 == ret)
        {
            printf("Created a receiver thread with ID '%lu'.\n",
                   receivers_pool[receivers_indx]);
            receivers_indx++;
        }
        else
        {
            printf("ERROR: failed to create a receiver thread: [%d]. "
                   "Contents of ID: '%lu'.\n",
                   ret, receivers_pool[receivers_indx]);
        }

        if(MAX_RECEIVERS_NUM == receivers_indx)
        {
            join_receivers(receivers_pool, MAX_RECEIVERS_NUM);
            receivers_indx = 0;
        }
    }
    while(curr_result = curr_result->next, curr_result != NULL);
    join_receivers(receivers_pool, receivers_indx);

    return 1;
}

static int join_receivers(pthread_t* receivers_pool, unsigned int num)
{
    if(NULL == receivers_pool) return 0;

    void* exit_status = NULL;

    for(unsigned int i = 0; i < num; i++)
    {
        int ret = pthread_join(receivers_pool[i], &exit_status);
        if(0 == ret && exit_status != NULL)
        {
            printf("Receiver thread '%lu' joined. Exit status: '%d'.\n",
                   receivers_pool[i], *((receiver_exit_status_t*)exit_status));
        }
        else
        {
            printf("ERROR: failed to join a receiver thread with "
                   "ID '%lu': [%d].\n",
                   receivers_pool[i], ret);
        }
    }
    return 1;
}

