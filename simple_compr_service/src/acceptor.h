#ifndef SCS_REQUESTS_ACCEPTOR
#define SCS_REQUESTS_ACCEPTOR

typedef enum acceptor_exit_status
{
    ACC_OK,
    ACC_ANOTHER_ERROR
} acceptor_exit_status_t;

typedef struct acceptor_args
{
    int server_socket_descr; // socket for incoming connections
    const int* still_listening; // current status of the service
    acceptor_exit_status_t exit_status; // argument for pthread_exit
} acceptor_args_t;

void* accept_requests(void* args);

#endif /* SCS_REQUESTS_ACCEPTOR */
