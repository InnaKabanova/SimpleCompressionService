#ifndef SCS_REQUEST_PROCESSOR
#define SCS_REQUEST_PROCESSOR

typedef enum acceptor_exit_status
{
    OK,

} acceptor_exit_status_t;

typedef struct acceptor_args
{
    int server_socket_descr; // to listen for connections
    const int* still_listening; // current status of the service
    acceptor_exit_status_t exit_status; // return var for pthread_exit
} acceptor_args_t;

void* accept_requests(void* args);
void* process_requests(void* args);
void* send_responses(void* args);

#endif /* SCS_REQUEST_PROCESSOR */
