#include "utilities.h"
#include "config.h"
#include "sender.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define TC_MAIN_DBG

// Max. number of worker threads that simultaneously read data from
// files, deserialize those data into requests and send those requests
// to a compression service:
#define MAX_SENDERS_NUM 3

static pthread_t senders_pool[MAX_SENDERS_NUM];
static sender_args_t args_pool[MAX_SENDERS_NUM];

int join_senders(unsigned int num);

int main(int argc, char* argv[])
{
    const char* node; // address of a target compression service

    //----------------------------------------------------------------
    // Parse the command line arguments and check their validity:
    //----------------------------------------------------------------
    if(argc < 2)
        exit_with_failure("not enough parameters. Hint: use --help");
    else if(argc > 3)
        exit_with_failure("too many parameters. Hint: use --help");

    // Evaluate the second parameter:
    // if "-h"/"--help"
    int help_requested =
            0 == strcmp(argv[1], "-h") || 0 == strcmp(argv[1], "--help");
    if(2 == argc && !help_requested)
        exit_with_failure("not enough parameters. Hint: use --help");
    else if(help_requested)
    {
        printf("1. test_client [serv-ip-addr] [serv-port-num]\n"
               "[serv-ip-addr] -- IP address of the target compression "
               "service. Valid values for connecting to a service "
               "running on the same host with the client: '127.0.0.1', "
               "'0.0.0.0', 'localhost'.\n"
               "[serv-port-num] -- port number of the target compression "
               "service.\n"
               "2. test_client --help (or -h)\n");
        exit(EXIT_SUCCESS);
    }
    // if [serv-ip-addr]
    if(0 == strcmp(argv[1], "localhost"))
        node = "0.0.0.0";
    else
        node = argv[1];

    // Evaluate the third parameter ([serv-port-num]):
    if(0 == atoi(argv[2])) // if conversion failed
        exit_with_failure("invalid service port number provided");

    //----------------------------------------------------------------
    // Spawn requests-sending threads:
    //----------------------------------------------------------------
    // 1 file with requests is processed by 1 worker thread.
    // Max. number of worker threads at a time is limited by
    // MAX_SENDERS_NUM.
    // Number of files with requests is not limited by the implementation.
    char* filepathes = NULL;
    if(0 == get_filepathes(&filepathes))
        exit_with_failure("failed to read the files list");

    char* token = NULL;
    const char delim[2] = "\n";
    unsigned int pool_ind = 0;
    token = strtok(filepathes, delim);
    while(NULL != token)
    {
#ifdef TC_MAIN_DBG
        printf( "Creating a worker thread to process '%s'.\n", token);
#endif
        args_pool[pool_ind].filepath = token;
        args_pool[pool_ind].node = node;
        args_pool[pool_ind].port_num = argv[2];
        args_pool[pool_ind].exit_status = OTHER_ERROR;
        int ret = pthread_create(&senders_pool[pool_ind], NULL,
                                 send_requests, &args_pool[pool_ind]);
#ifdef TC_MAIN_DBG
        if(0 == ret)
            printf("Created a sender thread with ID %lu.\n",
                   senders_pool[pool_ind]);
        else
            printf("ERROR: failed to create a sender thread. "
                   "Errno: %d. Contents of ID: %lu.\n", ret,
                   senders_pool[pool_ind]);
#endif
        pool_ind++;
        token = strtok(NULL, delim);
        if(pool_ind == MAX_SENDERS_NUM)
        {
            join_senders(pool_ind);
            pool_ind = 0;
        }
    }
    join_senders(pool_ind);
    free(filepathes);
    return EXIT_SUCCESS;
}

int join_senders(unsigned int num)
{
    if(num > MAX_SENDERS_NUM)
        return 0;

    for(unsigned int i = 0; i < num; i++)
    {
        int ret = pthread_join(senders_pool[i], NULL);
#ifdef TC_MAIN_DBG
        if(0 == ret)
            printf("Thread %lu joined. Exit status: %d.\n", senders_pool[i],
                   args_pool[i].exit_status);
        else
            printf("ERROR: failed to join a sender thread with ID %lu. "
                   "Errno: %d.\n", senders_pool[i], ret);
        // As worker threads write their return codes to the variables
        // on the main thread's stack, we access them directly
        // (in args structure), not via pthread_join's 2nd parameter.
#endif
    }
    return 1;
}
