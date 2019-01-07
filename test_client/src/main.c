#include "networking.h"
#include "sender.h"
#include "utilities.h"
#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define DEBUGGING
// Number of threads that read data from files, form requests based on
// that data and send those requests to the connected compression service:
#define SENDERS_NUM 6

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

    // Evaluate the second parameter ("-h"/"--help" or service's address):
    int help_requested =
            0 == strcmp(argv[1], "-h") || 0 == strcmp(argv[1], "--help");
    if(2 == argc && !help_requested)
        exit_with_failure("not enough parameters. Hint: use --help");
    else if(help_requested)
    {
        printf("1. test_client [serv-ip-addr] [serv-port-num]\n");
        printf("[serv-ip-addr] -- IP address of the target compression "
               "service. Valid values for connecting to a service "
               "running on the same host as the client: '127.0.0.1', "
               "'0.0.0.0', 'localhost'.");
        printf("[serv-port-num] -- port number of the target compression "
               "service.\n");
        printf("2. test_client --help (or -h)\n");
        exit(EXIT_SUCCESS);
    }
    if(0 == strcmp(argv[1], "localhost"))
        node = "0.0.0.0";
    else
        node = argv[1];

    // Evaluate the third parameter (service's port):
    if(0 == atoi(argv[2])) // conversion failed
        exit_with_failure("invalid service port number provided");

    //----------------------------------------------------------------
    // Spawn request-sending threads:
    //----------------------------------------------------------------
    pthread_t senders_pool[SENDERS_NUM];
    struct sender_args args_pool[SENDERS_NUM];
    int pool_ind = 0;
    char* files_string;

    parse_config_list(&files_string);
    const char delim[2] = "\n";
    char* token = strtok(files_string, delim);

    while(token != NULL)
    {
#ifdef DEBUGGING
        printf( "Creating a sender thread to read '%s'\n", token);
#endif
        args_pool[pool_ind].sock_descr = 42;
        args_pool[pool_ind].filepath = token;
        if(0 != pthread_create(&senders_pool[pool_ind], NULL,
                               send_requests, &args_pool[pool_ind]))
        {
#ifdef DEBUGGING
            printf( "Failed to create a sender thread\n");
#endif
            break;
        }
#ifdef DEBUGGING
        else
            printf("Created a sender thread with ID %lu.\n",
                   senders_pool[pool_ind]);
#endif
        pool_ind++;
        if(pool_ind >= SENDERS_NUM) break;
        token = strtok(NULL, delim);
    }

    //----------------------------------------------------------------
    // Wait for sender threads termination and then cleanup:
    //----------------------------------------------------------------
    for(int i = 0; i < SENDERS_NUM; i++)
        pthread_join(senders_pool[i], NULL);

    free(files_string);

    return EXIT_SUCCESS;
}
