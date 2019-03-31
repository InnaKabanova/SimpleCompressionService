#ifndef SCS_REQUEST_PROCESSOR
#define SCS_REQUEST_PROCESSOR

typedef enum worker_exit_status
{
    OK,
    OTHER_ERROR,

} worker_exit_status_t;

typedef struct acceptor_args
{
    int server_socket_descr; // socket for incoming connections
    const int* still_listening; // current status of the service
    worker_exit_status_t exit_status; // argument for pthread_exit
} acceptor_args_t;

typedef struct processor_args
{
    worker_exit_status_t exit_status;
} processor_args_t;

typedef struct sender_args
{
    worker_exit_status_t exit_status;
} sender_args_t;

void* accept_requests(void* args);
void* process_requests(void* args);
void* send_responses(void* args);

#endif /* SCS_REQUEST_PROCESSOR */
