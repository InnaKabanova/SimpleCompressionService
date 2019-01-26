#include "worker.h"
#include "requests_queue.h"

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

#include <pthread.h>

#define SCS_WORKER_DEBUGGING

void* accept_requests(void* args)
{
    if(!args)
        pthread_exit(NULL);
    acceptor_args_t* server_info = (acceptor_args_t*)args;
    if(!server_info->still_listening)
        pthread_exit(NULL); // TODO: exit & report an error here

    int clients_connected = 0;
    while(server_info->still_listening)
    {
#ifdef SCS_WORKER_DEBUGGING
        printf("Accepting... %d\n", clients_connected + 1); // TO REMOVE
#endif
        internal_request_t new_request =
        {.header = template_request_header};
        new_request.client_addr_len = sizeof(new_request.client_addr);

        int conn_socket = accept(server_info->server_socket_descr,
                          (struct sockaddr*)(&new_request.client_addr),
                          &new_request.client_addr_len);
        if(-1 == conn_socket)
        {
#ifdef SCS_WORKER_DEBUGGING
            printf("Error accepting connection on the socket %d\n",
                   server_info->server_socket_descr);
#endif
        }
        else
        {
#ifdef SCS_WORKER_DEBUGGING
            printf("Accepted connection on the socket %d. Obtained "
                   "a new socket: %d\n", server_info->server_socket_descr,
                   conn_socket);

            // TO REMOVE:
            shutdown(conn_socket, 2);
#endif
        }
        // TO REMOVE:
        clients_connected++;
        if(clients_connected == 6) break;
    }

    shutdown(server_info->server_socket_descr, 2);
    // TODO: exit & report SUCCESS here
}

void* process_requests(void* args)
{
    // TODO
}

void* send_responses(void* args)
{
    // TODO
}


