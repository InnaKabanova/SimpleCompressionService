#ifndef SIMPLE_COMPR_SERVICE_RECEIVER
#define SIMPLE_COMPR_SERVICE_RECEIVER

#define SCS_RECEIVER_DBG

typedef struct receiver_args
{
    unsigned long int thread_id;
    int sock_descr;
} receiver_args_t;

/**
 * @brief Worker thread which receives requests from a connected client
 * until the client is done & disconnected or timeout/error occurs.
 * @param[in] args: a receiver_args_t object, where 'thread_id' equals
 * to this thread's id (pthread_self) and 'sock_descr' is a handle for a
 * valid established connection with the client.
 */
void* receive_requests(void* args);

#endif /* SIMPLE_COMPR_SERVICE_RECEIVER */
