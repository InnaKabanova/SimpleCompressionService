#include "networking.h"
#include "sender.h"
#include "utilities.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define DEBUGGING

int main(int argc, char* argv[])
{
    const char* node;
    int sock_descr = -1;
    int senders_required = 1;

    int i;

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
    // Spawn client threads once a service connection is established:
    //----------------------------------------------------------------
    sock_descr = try_to_connect(node, argv[2]);
    if(-1 == sock_descr)
        exit_with_failure("could not connect to a specified service");

    // TODO: read configuration file with filepaths into dynamic
    // filename_array + create a dynamic array of sender threads
    // with random sleep time between sends.
    // for(i = 0; i < senders_required; i++)
    // {
    // }
    // TODO: consider quering of number of CPU cores to determine the
    // max. number of sender threads working at once
    // TODO: create sender threads detached or joinable?

    pthread_t test_sender;
    if(0 != pthread_create(&test_sender, NULL, send_requests, NULL))
        exit_with_failure("failed to create a sender thread");
#ifdef DEBUGGING
    else
        printf("Created a sender thread with ID %lu.\n", test_sender);
#endif







    return EXIT_SUCCESS;
}
