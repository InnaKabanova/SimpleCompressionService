#include "utilities.h"
#include "server.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#define SCS_SERVER_DEBUGGING 1

int start_server(int* sock_descr, const char* port_num, const int backlog)
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
    {
#ifdef SCS_SERVER_DEBUGGING
        printf("ERROR: %s.\n", gai_strerror(status));
        return 0;
#endif
    }

    // Loop through address lookup results and bind to the first we can:
    for(i = serv_info; i != NULL; i = i->ai_next)
    {
#ifdef SCS_SERVER_DEBUGGING
        print_addrinfo(i, "Possible bind", SCS_SERVER_DEBUGGING ? 1 : 0);
#endif
        if(-1 == (*sock_descr = socket(i->ai_family, i->ai_socktype,
                                      i->ai_protocol))) continue;
        status = setsockopt(*sock_descr, SOL_SOCKET, SO_REUSEADDR,
                            &(int){ 1 }, sizeof(int));
#ifdef SCS_SERVER_DEBUGGING
        if(status < 0)
            printf("ERROR: setsockopt(SO_REUSEADDR) failed.\n");
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
    {
#ifdef SCS_SERVER_DEBUGGING
        printf("ERROR: failed to bind to the local host's IP address.\n");
        return 0;
#endif
    }
    if(-1 == listen(*sock_descr, backlog))
    {
#ifdef SCS_SERVER_DEBUGGING
        printf("ERROR: failed to mark the obtained socket for listening. "
               "Errno: %d.\n", errno);
        return 0;
#endif
    }

    return 1;
}
