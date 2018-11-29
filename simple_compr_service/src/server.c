#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <pthread.h>

#include <sys/types.h> // data types used in system calls
#include <sys/socket.h> // structs for sockets
#include <netdb.h>
#include <arpa/inet.h>

#include "utilities.h"
#include "server.h"
#include "worker.h"

#define DEBUGGING
#define PROCESSORS_NUM 1

struct acceptor_args
{   
    int server_socket_descr; // to listen for connections
    const int* still_listening; // current status of the service
};

void initialize_networking(const char* port_num, int* sock_descr,
                           const int backlog);
void start_worker_threads();

//--------------------------------------------------------------------
// The boss routine/thread:
//--------------------------------------------------------------------
void start_server(const char* port_num, const int backlog)
{
    int sock_descr; // socket to listen for incoming connections
    int listening = 0;

    initialize_networking(port_num, &sock_descr, backlog);
    listening = 1;

}

//--------------------------------------------------------------------

void initialize_networking(const char* port_num, int* sock_descr,
                           const int backlog)
{
    struct addrinfo hints;
    struct addrinfo* serv_info;

    int status, counter = 1;
    struct addrinfo* i;

    memset((void*)&hints, 0, sizeof(hints));
    // The server will listen on the current host's IP address:[port_num]:
    hints.ai_family = AF_UNSPEC; // both IPv4 and IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE; // use the IP address of the local host

    if((status = getaddrinfo(NULL, port_num, &hints, &serv_info)) != 0)
        exit_with_failure(gai_strerror(status));
    // Loop through address lookup results and bind to the first we can:
    for(i = serv_info; i != NULL; i = i->ai_next)
    {
#ifdef DEBUGGING
        print_addrinfo(i, "Possible bind", 1);
#endif
        if(-1 == (*sock_descr = socket(i->ai_family, i->ai_socktype,
                                      i->ai_protocol)))
            continue;
        if(-1 == bind(*sock_descr, i->ai_addr, i->ai_addrlen))
        {
            close(*sock_descr);
            continue;
        }
        break;
    }
    freeaddrinfo(serv_info);

    if(NULL == i)
        exit_with_failure("failed to bind to local host's IP address");

    if(0 > listen(*sock_descr, backlog))
        exit_with_failure("failed to start listening for client "
                          "connections");
}

//--------------------------------------------------------------------

void start_worker_threads()
{

}

