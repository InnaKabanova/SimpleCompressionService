#include "connection.h"
#include "utilities.h"

#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>

// #define TC_NETWORKING_DBG 1

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
    {
#ifdef TC_NETWORKING_DBG
        printf("From %lu | ERROR: %s.\n", pthread_self(), gai_strerror(status));
#endif
    }

    // Loop through address lookup results and connect to the first we can:
    for(i = serv_info; i != NULL; i = i->ai_next)
    {
#ifdef TC_NETWORKING_DBG
        print_addrinfo(i, "Possible target host",
                       TC_NETWORKING_DBG ? 1 : 0);
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
