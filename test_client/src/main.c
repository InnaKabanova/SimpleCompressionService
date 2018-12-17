#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h> // data types used in system calls
#include <sys/socket.h> // structs for sockets
#include <netdb.h>

#include "utilities.h"

#define DEBUGGING 1 // true/false

int try_to_connect(const char* node, const char* port_num);

int main(int argc, char* argv[])
{
    const char* node;
    int sock_descr;

    //----------------------------------------------------------------
    // Parse the command line arguments and check their validity:
    //----------------------------------------------------------------
    if(argc < 2)
        exit_with_failure("not enough parameters. Hint: use --help");
    else if(argc > 3)
        exit_with_failure("too many parameters. Hint: use --help");

    // Evaluate the second parameter:
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

    if(0 == atoi(argv[2])) // conversion failed
        exit_with_failure("invalid service port number provided");


    if(0 == strcmp(argv[1], "localhost"))
        node = "0.0.0.0";
    else
        node = argv[1];

    //----------------------------------------------------------------
    //
    //----------------------------------------------------------------
    sock_descr = try_to_connect(node, argv[2]);





    return EXIT_SUCCESS;
}

int try_to_connect(const char* node, const char* port_num)
{
    int sock_descr = -1;
    struct addrinfo hints;
    struct addrinfo* serv_info;

    int status;
    struct addrinfo* i;

    memset((void*)&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets

    if((status = getaddrinfo(node, port_num, &hints, &serv_info)) != 0)
        exit_with_failure(gai_strerror(status));

    // Loop through address lookup results and connect to the first we can:
    for(i = serv_info; i != NULL; i = i->ai_next)
    {
#ifdef DEBUGGING
        print_addrinfo(i, "Possible target host", DEBUGGING ? 1 : 0);
#endif
        if(-1 == (sock_descr = socket(i->ai_family, i->ai_socktype,
                                      i->ai_protocol)))
            continue;
        if(-1 == connect(sock_descr, i->ai_addr, i->ai_addrlen))
        {
            close(sock_descr);
            sock_descr = -1;
            continue;
        }
        break;
    }
    freeaddrinfo(serv_info);
    return sock_descr;
}

