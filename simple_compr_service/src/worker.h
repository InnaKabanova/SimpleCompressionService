#ifndef SCS_REQUEST_PROCESSOR
#define SCS_REQUEST_PROCESSOR

struct acceptor_args
{
    int server_socket_descr; // to listen for connections
    const int* still_listening; // current status of the service
};

void* accept_requests(void* args);
void* process_requests(void* args);
void* send_responses(void* args);

#endif /* SCS_REQUEST_PROCESSOR */
