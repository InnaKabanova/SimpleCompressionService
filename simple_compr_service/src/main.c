#include "utilities.h"
#include "server.h"
#include "worker.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>

#define SCS_MAIN_DEBUGGING

// Max. number of pending incoming connections:
#define BACKLOG 10
// Number of worker threads that process incoming requests:
#define PROCESSORS_NUM 3

void wait_for_term_input(const char* cmd);
void signals_handler(int signum);

static int listening = 0;

int main(int argc, char* argv[])
{
    int sock_descr = -1;

    //----------------------------------------------------------------
    // Parse the command line arguments and check their validity:
    //----------------------------------------------------------------
    if(argc > 2)
        exit_with_failure("too many parameters. Use --help");
    else if(argc < 2)
        exit_with_failure("not enough parameters. Use --help");

    if(0 == strcmp(argv[1], "-h") || 0 == strcmp(argv[1], "--help"))
    {
        printf("1. simple_compr_service [port_number]\n");
        printf("[port_number] -- port for incoming connections.\n");
        printf("2. simple_compr_service --help (or -h)\n");
        exit(EXIT_SUCCESS);
    }
    else if(0 == atoi(argv[1])) // conversion failed
        exit_with_failure("invalid port number provided");

    //----------------------------------------------------------------
    // Start the networking:
    //----------------------------------------------------------------
#ifdef SCS_MAIN_DEBUGGING
    printf("Binding to port %s ...\n", argv[1]);
#endif
    if(0 == start_server(&sock_descr, argv[1], BACKLOG))
        exit_with_failure("failed to initialize networking");
    listening = 1;

    //----------------------------------------------------------------
    // Create worker threads, launch incoming connections/requests
    // processing:
    //----------------------------------------------------------------
    pthread_t acceptor; // listens for & accepts incoming connections
    pthread_t processors_pool[PROCESSORS_NUM]; // deal with incoming requests
    pthread_t sender; // sends responses (produced by processor threads) to clients

    acceptor_args_t accptr_args = { .server_socket_descr = sock_descr,
                                    .still_listening = &listening,
                                    .exit_status = OTHER_ERROR};
    processor_args_t prcssr_args_pool[PROCESSORS_NUM];
    for(int i = 0; i < PROCESSORS_NUM; i++)
        prcssr_args_pool[i].exit_status = OTHER_ERROR;
    sender_args_t sndr_args = {.exit_status = OTHER_ERROR};

    if(0 != pthread_create(&sender, NULL, send_responses,
                           (void*)&sndr_args))
        exit_with_failure("failed to create a sender thread");
#ifdef SCS_MAIN_DEBUGGING
    else
        printf("Created a sender thread with ID %lu.\n", sender);
#endif
    for(int i = 0; i < PROCESSORS_NUM; i++)
    {
        if(0 != pthread_create(&processors_pool[i], NULL, process_requests,
                               (void*)&prcssr_args_pool[i]))
            exit_with_failure("failed to create a processor thread");
#ifdef SCS_MAIN_DEBUGGING
        else
            printf("Created a processor thread #%i with ID %lu.\n", i + 1,
                   processors_pool[i]);
#endif
    }
    if(0 != pthread_create(&acceptor, NULL, accept_requests,
                           (void*)&accptr_args))
        exit_with_failure("failed to create an acceptor thread");
#ifdef SCS_MAIN_DEBUGGING
    else
        printf("Created an acceptor thread with ID %lu.\n", acceptor);
#endif

    //----------------------------------------------------------------
    // Wait for user input to terminate the service:
    //----------------------------------------------------------------

    // TODO: Another blocking function here.
    // Implement boss + workers synchronization.
    wait_for_term_input("exit");
    listening = 0;

    //----------------------------------------------------------------
    // Make all the necessary cleanup before termination:
    //----------------------------------------------------------------
    pthread_join(acceptor, NULL);
    for(int i = 0; i < PROCESSORS_NUM; i++)
        pthread_join(processors_pool[i], NULL);
    pthread_join(sender, NULL);
    // TODO: printf threads' return codes with consideration of
    // thread cancellation.

    return EXIT_SUCCESS;
}

void signals_handler(int signum)
{
    if(signum == SIGINT) // Interrupt, Ctrl-C
    {
        // Do nothing.
    }
}

void wait_for_term_input(const char* cmd)
{
    char buffer[5] = {'s','t','a','r','t'};
    printf("SUCCESS: the service is now running...\n"
           "Type 'exit' to terminate it.\n");
    while(strcmp((const char*)&buffer, cmd) != 0)
    {
        fgets(buffer, 5, stdin);
        printf("\n");
    }
}
