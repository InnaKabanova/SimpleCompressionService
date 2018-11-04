#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h> // data types used in system calls
#include <sys/socket.h> // structs for sockets
#include <netinet/in.h> // consts & structs for Internet domain addresses
#include <arpa/inet.h>

#include "utilities.h"
#include "server.h"
#include "processor.h"
#include "request.h"

#define VERBOSE_SERVER
#define WORKERS_NUM 1

void* accept_requests(void* args);
void* send_responses(void* args);

struct acceptor_args
{   
    int server_socket_descr; // to listen for connections
    const int* still_listening; // current status of the service
};

// The main/boss thread:
void start_server(int port_num, int backlog)
{
    // Networking initialization:
    int socket_descr; // socket to listen for incoming connections
    struct sockaddr_in server_addr;
    int listening = 0;

    if((socket_descr = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        exit_with_failure("failed to get a socket");

    memset((void*)&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET; // using Internet domain address
    server_addr.sin_addr.s_addr = INADDR_ANY; // bind to all local interfaces
    server_addr.sin_port = htons(port_num);

    if(bind(socket_descr, (struct sockaddr*)&server_addr,
            sizeof(server_addr)) < 0)
        exit_with_failure("failed to bind the socket to the address");

    if(listen(socket_descr, backlog) < 0)
    {
#ifdef VERBOSE_SERVER
        printf("ERROR: %s.\n", strerror(errno));
#endif /* VERBOSE_SERVER */
        exit_with_failure("failed to start listening for client connections");
    }
    listening = 1;

    //----------------------------------------------------------------

    // Creating worker threads:
    pthread_t acceptor; // listens for & accepts incoming connections
    pthread_t processors_pool[WORKERS_NUM]; // deal with incoming requests
    pthread_t sender; // sends responses (produced by processor threads) to clients

    int i, thread_status;
    //Start a sender thread:
    if(thread_status = pthread_create(&sender, NULL, send_responses, NULL))
    {
#ifdef VERBOSE_SERVER
        printf("ERROR: %i.\n", thread_status);
#endif /* VERBOSE_SERVER */
        exit_with_failure("failed to create a sender thread");
    }
    else
    {
#ifdef VERBOSE_SERVER
        printf("Created a sender thread with ID %lu.\n", sender);
#endif /* VERBOSE_SERVER */
    }

    // Start processor threads:
    for(i = 0; i < WORKERS_NUM; i++)
    {
        if(thread_status = pthread_create(&processors_pool[i], NULL,
                                          process_requests, NULL))
        {
#ifdef VERBOSE_SERVER
            printf("ERROR: %i. Thread #%i.\n", thread_status, i);
#endif /* VERBOSE_SERVER */
            exit_with_failure("failed to create a processor thread");
        }
        else
        {
#ifdef VERBOSE_SERVER
            printf("Created a processor thread #%i with ID %lu.\n", i + 1,
                   processors_pool[i]);
#endif /* VERBOSE_SERVER */
        }
    }

    // Start an acceptor thread:
    struct acceptor_args accptr_args = { .server_socket_descr = socket_descr,
                                         .still_listening = &listening};
    if(thread_status = pthread_create(&acceptor, NULL, accept_requests,
                                      (void*)&accptr_args))
    {
#ifdef VERBOSE_SERVER
        printf("ERROR: %i.\n", thread_status);
#endif /* VERBOSE_SERVER */
        exit_with_failure("failed to create an acceptor thread");
    }
    else
    {
#ifdef VERBOSE_SERVER
        printf("Created an acceptor thread with ID %lu.\n", acceptor);
#endif /* VERBOSE_SERVER */
    }

    //----------------------------------------------------------------

    // Waiting for user input to terminate the service:
    char buffer[5] = {'s','t','a','r','t'};
    printf("SUCCESS: the service is now running...\n "
           "Type 'exit' to terminate it.\n");
    while(strcmp((const char*)&buffer, "exit") != 0)
    {
        fgets(buffer, 5, stdin);
        printf("\n");
    }

    listening = 0;

    //----------------------------------------------------------------

    // Cleanup:
    pthread_cancel(acceptor);
    for(i = 0; i < WORKERS_NUM; i++)
        pthread_cancel(processors_pool[i]);
    pthread_cancel(sender);

}

void* accept_requests(void* args)
{
    struct acceptor_args* service_info = (struct acceptor_args*)args;

    struct sockaddr new_client_addr; // to know where to send a response
    socklen_t client_addr_size = sizeof(struct sockaddr);
    struct request_header new_request_header;
    size_t request_header_size = sizeof(struct request_header);
    int new_socket_descr; // for active connection with a new client

    while(*service_info->still_listening)
    {
        memset((void*)&new_client_addr, 0, client_addr_size);
        memset((void*)&new_request_header, 0, request_header_size);

        new_socket_descr = accept(service_info->server_socket_descr,
                                  &new_client_addr, &client_addr_size);

        printf("SUCCESS (got a client connection)");
        break;

        // Each incoming request is received in 2 steps.
        // 1. Receiving a header:
        ssize_t bytes_received = recv(new_socket_descr, (void*)&new_request_header,
                                  request_header_size, 0);
        // Handle errors if any:
        if(bytes_received != request_header_size ||
           new_request_header.magic_value != DEFAULT_MAGIC_VALUE)
        {
            // TODO: skip the whole package and data that follows
            // TODO: learn how to do regulation
        }
    }
}

void* send_responses(void* args)
{

}






































