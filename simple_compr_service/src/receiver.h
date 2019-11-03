#ifndef SIMPLE_COMPR_SERVICE_RECEIVER
#define SIMPLE_COMPR_SERVICE_RECEIVER

#define SCS_RECEIVER_DBG

typedef struct receiver_args
{
    unsigned long int thread_id;
    int sock_descr;
} receiver_args_t;

/**
 * @brief
 * @param[in] args: .
 */
void* receive_requests(void* args);

#endif /* SIMPLE_COMPR_SERVICE_RECEIVER */
