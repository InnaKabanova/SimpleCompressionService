#include "acceptor.h"
#include "requests_queue.h"

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

#include <pthread.h>

#define SCS_ACCEPTOR_DEBUGGING

void* accept_requests(void* args)
{
    if(!args)
        pthread_exit(NULL);
    acceptor_args_t* thread_io = (acceptor_args_t*)args;
    if(!thread_io->still_listening)
    {
        thread_io->exit_status = ACC_ANOTHER_ERROR;
        pthread_exit((void*)(&thread_io->exit_status));
    }

    while(thread_io->still_listening)
    {
        scs_internal_request_t new_request; // TODO: on heap
        new_request.client_addr_len = sizeof(new_request.client_addr);

        int conn_socket = accept(thread_io->server_socket_descr,
                          (struct sockaddr*)(&new_request.client_addr),
                          &new_request.client_addr_len);
        if(-1 == conn_socket)
        {
#ifdef SCS_ACCEPTOR_DEBUGGING
            printf("Error accepting connection on the socket %d\n",
                   thread_io->server_socket_descr);
#endif
        }
        else
        {
#ifdef SCS_ACCEPTOR_DEBUGGING
            printf("Accepted connection on the socket %d. Obtained "
                   "a new socket: %d\n", thread_io->server_socket_descr,
                   conn_socket);
#endif
        }
    }

    shutdown(thread_io->server_socket_descr, 2);
    thread_io->exit_status = ACC_OK;
    pthread_exit((void*)(&thread_io->exit_status));
}

