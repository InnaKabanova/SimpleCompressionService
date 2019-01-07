#include "utilities.h"
#include "server.h"

#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#define DEBUGGING 1

void start_server(int* sock_descr, const char* port_num, const int backlog)
{
    struct addrinfo hints;
    struct addrinfo* serv_info;

    int status;
    struct addrinfo* i;

    memset((void*)&hints, 0, sizeof(hints));
    // The server will listen on the current host's IP address:[port_num]:
    hints.ai_family = AF_UNSPEC; // both IPv4 and IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE; // use the IP address of the local host

    if((status = getaddrinfo(NULL, port_num, &hints, &serv_info)) != 0)
        exit_with_failure(gai_strerror(status));

    // Loop through address lookup results and bind to the first we can:
    for(i = serv_info; i != NULL; i = i->ai_next)
    {
#ifdef DEBUGGING
        print_addrinfo(i, "Possible bind", DEBUGGING ? 1 : 0);
#endif
        if(-1 == (*sock_descr = socket(i->ai_family, i->ai_socktype,
                                      i->ai_protocol))) continue;
        status = setsockopt(*sock_descr, SOL_SOCKET, SO_REUSEADDR,
                            &(int){ 1 }, sizeof(int));
#ifdef DEBUGGING
        if(status < 0) printf("setsockopt(SO_REUSEADDR) failed");
#endif
        if(-1 == bind(*sock_descr, i->ai_addr, i->ai_addrlen))
        {
            close(*sock_descr);
            continue;
        }
        break;
    }

    freeaddrinfo(serv_info);
    if(NULL == i)
        exit_with_failure("failed to bind to the local host's IP address");
    if(-1 == listen(*sock_descr, backlog))
        exit_with_failure("failed to start listening for client "
                          "connections");
}
