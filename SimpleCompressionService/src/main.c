#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utilities.h"
#include "server.h"

#define VERBOSE_MAIN

int main(int argc, char* argv[])
{
    // Parse the command line arguments:
    if(argc != 2)
        exit_with_failure("invalid input parameters");
    else if(!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))
    {
        printf("1. simple_compr_service [port_number]\n");
        printf("[port_number] -- port to bind the service to.\n");
        printf("2. simple_compr_service --help (or -h)\n");
        exit(EXIT_SUCCESS);
    }
    else
    {
        int port_number = atoi(argv[1]);
        if(0 == port_number) // conversion failed
            exit_with_failure("invalid port number provided");
        else
        {
#ifdef VERBOSE_MAIN
            printf("Creating a server bound to port %i...\n", port_number);
#endif /*VERBOSE_MAIN*/
            start_server(port_number);
        }
    }
    return EXIT_SUCCESS;
}
