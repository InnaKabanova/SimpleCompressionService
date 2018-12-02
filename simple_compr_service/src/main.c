#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "utilities.h"
#include "server.h"

#define BACKLOG 10 // max. num of pending connections

void signals_handler(int signum);

int main(int argc, char* argv[])
{
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
    // Start the service:
    //----------------------------------------------------------------
    printf("Binding to port %s...\n", argv[1]);
    start_server(argv[1], BACKLOG);

    return EXIT_SUCCESS;
}

void signals_handler(int signum)
{
    if(signum == SIGINT) // Interrupt, Ctrl-C
    {
        // Do nothing.
    }
}
