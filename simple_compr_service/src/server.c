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
void wait_for_term_input(const char* cmd);

//--------------------------------------------------------------------
// The boss routine/thread:
//--------------------------------------------------------------------
void start_server(const char* port_num, const int backlog)
{
    int sock_descr;
    // TODO: consider making a pthread conditional variable out of this:
    int listening = 0;

    // Worker threads:
    pthread_t acceptor; // listens for & accepts incoming connections
    pthread_t processors_pool[PROCESSORS_NUM]; // deal with incoming requests
    pthread_t sender; // sends responses (produced by processor threads) to clients

    //----------------------------------------------------------------
    // Start listening for incoming connections:
    //----------------------------------------------------------------
    initialize_networking(port_num, &sock_descr, backlog);
    listening = 1;

    //----------------------------------------------------------------
    // Launch incoming connections/requests processing:
    //----------------------------------------------------------------
    int i;
    struct acceptor_args accptr_args = { .server_socket_descr = sock_descr,
                                         .still_listening = &listening};

    if(0 != pthread_create(&sender, NULL, send_responses, NULL))
        exit_with_failure("failed to create a sender thread");
#ifdef DEBUGGING
    else
        printf("Created a sender thread with ID %lu.\n", sender);
#endif
    for(i = 0; i < PROCESSORS_NUM; i++)
    {
        if(0 != pthread_create(&processors_pool[i], NULL, process_requests,
                               NULL))
            exit_with_failure("failed to create a processor thread");
#ifdef DEBUGGING
        else
            printf("Created a processor thread #%i with ID %lu.\n", i + 1,
                   processors_pool[i]);
#endif
    }
    if(0 != pthread_create(&acceptor, NULL, accept_requests,
                           (void*)&accptr_args))
        exit_with_failure("failed to create an acceptor thread");
#ifdef DEBUGGING
    else
        printf("Created an acceptor thread with ID %lu.\n", acceptor);
#endif

    //----------------------------------------------------------------
    // Wait for user input to terminate the service:
    //----------------------------------------------------------------
    wait_for_term_input("exit");
    listening = 0;

    //----------------------------------------------------------------
    // Make all the necessary cleanup before termination:
    //----------------------------------------------------------------
    pthread_cancel(acceptor);
    for(i = 0; i < PROCESSORS_NUM; i++)
        pthread_cancel(processors_pool[i]);
    pthread_cancel(sender);
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
        exit_with_failure("failed to bind to the local host's IP address");

    if(-1 == listen(*sock_descr, backlog))
        exit_with_failure("failed to start listening for client "
                          "connections");
}

//--------------------------------------------------------------------

void wait_for_term_input(const char* cmd)
{
    char buffer[5] = {'s','t','a','r','t'};
    printf("SUCCESS: the service is now running...\n "
           "Type 'exit' to terminate it.\n");
    while(strcmp((const char*)&buffer, cmd) != 0)
    {
        fgets(buffer, 5, stdin);
        printf("\n");
    }
}

